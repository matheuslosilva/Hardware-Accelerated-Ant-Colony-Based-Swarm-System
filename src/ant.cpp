#include <ant.h>
#include <iostream>

/* 
	TODO LIST:
	
	1 - CONSERTAR SEED - OK
	2 - FAZER A FORMIGA IR ATE O CENTRO ANTES DE VOLTAR OK
	3 - range para que o sensor perceba o ninho e a comida (feromonio da comida e do ninho) OK
	4 - Timeout/lifetime voltar a ser explorer OK
	5 - Se explorer encontrar trilha verde vira nestcarriercopia OK
*/
Ant::Ant(float x, float y, float theta, float size, float velocity)
{
	_x = x; //+ (float)(rand()%100-50)/SCR_WIDTH;
	_y = y; //+ (float)(rand()%100-50)/SCR_HEIGHT;

	_xSensorR =0;
	_ySensorR = 0;

	_xSensorL = 0;
	_ySensorL = 0;

	_theta = theta;
	_size = size;
	_velocity = velocity;
	_pheromoneType = 1;
	_placePheromoneIntensity = 60;
	_state = EXPLORER;
	_lifeTime = 0;
}

void Ant::move(int l)
{
	_lifeTime++;
	int angle = (int)((_theta/M_PI)*1800);
	if(angle < 0) angle += 3600;
	if(angle >= 3600) angle -= 3600;

	_x += _velocity*cosLookup[angle];
	_y += _velocity*sinLookup[angle];

	angle = (int)(((_theta+M_PI/4)/M_PI)*1800);
	if(angle < 0) angle += 3600;
	if(angle >= 3600) angle -= 3600;

	_xSensorR = _x + 0.02*cosLookup[angle]; // 1 pixel = 0.005 in size
	_ySensorR = _y + 0.02*sinLookup[angle];
	//      X
	//
	//
	//   O      O

	angle = (int)(((_theta-M_PI/4)/M_PI)*1800);
	if(angle < 0) angle += 3600;
	if(angle >= 3600) angle -= 3600;
	
	_xSensorL = _x + 0.02*cosLookup[angle];
	_ySensorL = _y + 0.02*sinLookup[angle];
	
	
	//Border treatment
	if(_x < -0.900f || _x > 0.900f) 
	{
		_x = _x < 0 ? -0.900f : 0.900f;	
	} 

	if(_y < -0.900f || _y > 0.900f) 
	{
		_y = _y < 0 ? -0.900f : 0.900f;
	}

}

void Ant::environmentAnalysis(int viewFrequency, vector<uint8_t> &pheromoneMatrix, vector<AntColony*> antColonies, vector<FoodSource*> foodSources)
{
	if(viewFrequency % 30 == 0)
	{
		int rSensorRedPheromoneDetected = 0;
		int lSensorRedPheromoneDetected = 0;
		int rSensorBluePheromoneDetected = 0;
		int lSensorBluePheromoneDetected = 0;
		int rSensorGreenPheromoneDetected = 0;
		int lSensorGreenPheromoneDetected = 0;

		int xn, yn, index;

		xn = ((SCR_WIDTH/2) + _xSensorR * (SCR_WIDTH/2));
		yn = ((SCR_HEIGHT/2) + _ySensorR * (SCR_HEIGHT/2));


		for(int i = -2; i <=2; i++)
		{
			for(int j = -2; j <= 2; j++)
			{
				index = ((yn+i) * SCR_WIDTH + (xn+j)) * 4;
				rSensorRedPheromoneDetected += pheromoneMatrix[index];
				rSensorGreenPheromoneDetected += pheromoneMatrix[index+1];
	    		rSensorBluePheromoneDetected += pheromoneMatrix[index+2];
			}
		}

		//rSensorRedPheromoneDetected +=  (rand()%6)-3;
		//rSensorBluePheromoneDetected +=  (rand()%6)-3;
		//rSensorGreenPheromoneDetected += (rand()%6)-3;

		xn = ((SCR_WIDTH/2) + _xSensorL * (SCR_WIDTH/2));
		yn = ((SCR_HEIGHT/2) + _ySensorL * (SCR_HEIGHT/2));
		

		for(int i = -2; i <=2; i++)
		{
			for(int j = -2; j <= 2; j++)
			{
				index = ((yn+i) * SCR_WIDTH + (xn+j)) * 4;
				lSensorRedPheromoneDetected += pheromoneMatrix[index];
				lSensorGreenPheromoneDetected += pheromoneMatrix[index+1];
	    		lSensorBluePheromoneDetected += pheromoneMatrix[index+2];
			}
		}

		//lSensorRedPheromoneDetected +=  (rand()%6)-3;
		//lSensorBluePheromoneDetected +=  (rand()%6)-3;
		//lSensorGreenPheromoneDetected +=  (rand()%6)-3;

		makeDecision(antColonies, foodSources, 
			lSensorRedPheromoneDetected, lSensorGreenPheromoneDetected, lSensorBluePheromoneDetected, 
			rSensorRedPheromoneDetected, rSensorGreenPheromoneDetected, rSensorBluePheromoneDetected);
		
		// Border Treatment
		if(_xSensorL < -0.900f || _xSensorL > 0.900f || _ySensorL < -0.900f || _ySensorL > 0.900f) _theta += glm::radians((float)(rand()%360)/10.0f-1.0f)*4.0f;
		else if(_xSensorR < -0.900f || _xSensorR > 0.900f || _ySensorR < -0.900f || _ySensorR > 0.900f) _theta -= glm::radians((float)(rand()%360)/10.0f-1.0f)*4.0f;

		if(_theta < 0) _theta += 2*M_PI;
		if(_theta >= 2*M_PI) _theta -= 2*M_PI;
	}		
}

bool Ant::foodColision(vector<FoodSource*> foodSources)
{
	for(int i = 0; i < foodSources.size(); i++)
	{
		if(foodSources[i]->antColision(_x, _y))
		{
			_x = foodSources[i]->posX;
			_y = foodSources[i]->posY;
			_carryingFood = true;	
			return true;
		}
	}
	return false;
}

bool Ant::nestColision(vector<AntColony*> antColonies)
{
	for(int i = 0; i < antColonies.size(); i++)
	{
		if(antColonies[i]->antColision(_x, _y))
		{	
			if (_carryingFood) {
				antColonies[i]->food += 1;
			}
			_x = antColonies[i]->posX;
			_y = antColonies[i]->posY;
			_carryingFood = false;
			return true;
		}
	}
	return false;
}

void Ant::changeState(States newState)
{
	switch(newState)
	{
		case EXPLORER:

			_state = EXPLORER;
			_pheromoneType = 1;
			_placePheromoneIntensity = 60; 
			 	
		break;

		case BACKHOME:

			_theta += glm::radians((float)(180.0f));
			_state = BACKHOME;
			_pheromoneType = -1;
			 	
		break;

		case CARRIER:

			_state = CARRIER;
			_pheromoneType = 2;
			_placePheromoneIntensity = 60;

		break;

		case NESTCARRIER:

			_state = NESTCARRIER;
			_pheromoneType = 2;
			_placePheromoneIntensity = 120;		

		break;

		case FOLLOWGREEN:

			_state = FOLLOWGREEN;
			_pheromoneType = -1;

		break;

		default:
		break;
	}
}

void Ant::makeDecision(vector<AntColony*> antColonies, vector < FoodSource* > foodSources, int lR, int lG, int lB, int rR, int rG, int rB)
{
	switch(_state)
	{
		case EXPLORER:
	
			if(rR > lR)
				_theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else  if(rR < lR)
				_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;

			if(rG > 0 || lG > 0)
			{
				changeState(FOLLOWGREEN);
			}

			if(foodColision(foodSources))
			{
				_theta += glm::radians((float)(180.0f));	
				_lifeTime = 0;

				changeState(CARRIER);
			}
		
			 	
		break;

		case BACKHOME:

			if(rR > lR)
				_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else  if(rR < lR)
				_theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			
			if(nestColision(antColonies))
			{			
				_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
				_theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
				_lifeTime = 0;

				changeState(EXPLORER);
			}
			 	
		break;

		case CARRIER:

			if(rG  > lG)
				_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rG < lG)
				_theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;

			if(rR > lR)
				_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rR < lR)
				_theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;

			if(nestColision(antColonies))
			{				
				_theta += glm::radians((float)(180.0f));
				_lifeTime = 0;
				
				changeState(NESTCARRIER);
			}
			break;

		case NESTCARRIER:

			if(rG  > lG)
				_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rG < lG)
				_theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			
			if(_carryingFood == true && nestColision(antColonies))
			{
				_lifeTime = 0;	
				_theta += glm::radians((float)(180.0f));
				_placePheromoneIntensity = 100;	
			}

			else if(_carryingFood == false && foodColision(foodSources))
			{
				_lifeTime = 0;
				_theta += glm::radians((float)(180.0f));
				_placePheromoneIntensity = 200;
			}
			break;

		case FOLLOWGREEN:
			if(rG  > lG)
				_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rG < lG)
				_theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			
			if(nestColision(antColonies))
			{	
				changeState(EXPLORER);
			}

			else if(foodColision(foodSources))
			{
				changeState(CARRIER);		
			}
		break;
	}

	//if(_lifeTime%50 == 0) _placePheromoneIntensity = max(0, _placePheromoneIntensity - (int)(_lifeTime*0.0025));
	if(_lifeTime >= 2500)
	{
		_lifeTime = 0;
		if(_state != BACKHOME) changeState(BACKHOME);
	}
}

	
