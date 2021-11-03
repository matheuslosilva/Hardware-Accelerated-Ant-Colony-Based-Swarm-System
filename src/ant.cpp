#include <ant.h>
#include <iostream>

/* 
	TODO LIST:
	
	CONSERTAR SEED
	FAZER A FORMIGA IR ATE O CENTRO ANTES DE VOLTAR 
	1- range para que o sensor perceba o ninho e a comida (feromonio da comida e do ninho)
	2- Timeout/lifetime voltar a ser explorer
	3- Se explorer encontrar trilha verde vira nestcarriercopia

*/
Ant::Ant(float x, float y, float theta, float size, float velocity, float placedPheromoneIntensity)
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
	_placePheromoneIntensity = placedPheromoneIntensity;
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
	/*
		   O         O	
			    
				
				
		    	    			
				X
	*/


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

void Ant::environmentAnalysis(int viewFrequency, vector<uint8_t> &pheromoneMatrix, float posXNest, float posYNest, vector<FoodSource*> foodSources)
{
	if(viewFrequency % 30 == 0)
	{
		if(_lifeTime >= 2000)
		{
			_lifeTime = 0;
			//_x = posXNest;
			//_y = posYNest;
			_state = EXPLORER2;
			_theta += glm::radians((float)(180.0f));
		}
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

		changeState(posXNest, posYNest, foodSources, 
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
		if(foodSources[i]->antColision(_x, _y)) return true;
	}
	return false;
}

void Ant::changeState(float posXNest, float posYNest, vector < FoodSource* > foodSources, int lR, int lG, int lB, int rR, int rG, int rB)
{
	/*

*/
	switch(_state)
	{
		case EXPLORER:
			_state = EXPLORER;
			_pheromoneType = 1;
			_placePheromoneIntensity = 60; // TODO

			if(rG > 0 || lG > 0)
			{
				_state = FOLLOWGREEN;
				_pheromoneType = -1;
			}
			
			if(rR > lR)
				_theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else  if(rR < lR)
				_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;

			

			if(foodColision(foodSources))
			{
				_state = CARRIER;
				_pheromoneType = 2;
				_placePheromoneIntensity = 20;

				_carryingFood = true;
				_theta += glm::radians((float)(180.0f));	
				_lifeTime = 0;
			}
		
			 	
		break;

		case EXPLORER2:
			_state = EXPLORER2;
			_pheromoneType = -1;
			_placePheromoneIntensity = 60; // TODO

			if(rG > 0 || lG > 0)
			{
				_state = FOLLOWGREEN;
				_pheromoneType = -1;
			}
			
			if(rR > lR)
				_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else  if(rR < lR)
				_theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;

			

			if(foodColision(foodSources))
			{
				_state = CARRIER;
				_pheromoneType = 2;
				_placePheromoneIntensity = 20;

				_carryingFood = true;
				_theta += glm::radians((float)(180.0f));	
				_lifeTime = 0;
			}
			
			if( _x >= (posXNest-(float)(25.0f/SCR_WIDTH)) 
			 && _x <= (posXNest+(float)(25.0f/SCR_WIDTH)) 
			 && _y >= (posYNest-(float)(25.0f/SCR_WIDTH))
			 && _y <= (posYNest+(float)(25.0f/SCR_WIDTH)))
			{
				_state = EXPLORER;
				_pheromoneType = 1;
				_placePheromoneIntensity = 60;
				
				_carryingFood = false;			
				_theta += glm::radians((float)(180.0f));
				_lifeTime = 0;
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


			if( _x >= (posXNest-(float)(25.0f/SCR_WIDTH)) 
			 && _x <= (posXNest+(float)(25.0f/SCR_WIDTH)) 
			 && _y >= (posYNest-(float)(25.0f/SCR_WIDTH))
			 && _y <= (posYNest+(float)(25.0f/SCR_WIDTH)))
			{
				_state = NESTCARRIER;
				_pheromoneType = 2;
				_placePheromoneIntensity = 100;
				
				_carryingFood = false;			
				_theta += glm::radians((float)(180.0f));
				_lifeTime = 0;
			}

		break;

		case NESTCARRIER:
			if(rG  > lG)
				_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rG < lG)
				_theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			
			if( _x >= (posXNest-(float)(25.0f/SCR_WIDTH)) 
			 && _x <= (posXNest+(float)(25.0f/SCR_WIDTH)) 
			 && _y >= (posYNest-(float)(25.0f/SCR_WIDTH))
			 && _y <= (posYNest+(float)(25.0f/SCR_WIDTH)))
			{
				if(_carryingFood == true)
				{
					_lifeTime = 0;
					_theta += glm::radians((float)(180.0f));
				}
				_placePheromoneIntensity = 100;
				_carryingFood = false;		
			}

			else if(foodColision(foodSources))
			{
				if(_carryingFood == false)
				{
					_lifeTime = 0;
					_theta += glm::radians((float)(180.0f));
				}
				_placePheromoneIntensity = 200;
				_carryingFood = true;
					
			}
			

		break;

		case FOLLOWGREEN:

			if(rG  > lG)
				_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rG < lG)
				_theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			
			if( _x >= (posXNest-(float)(25.0f/SCR_WIDTH)) 
			 && _x <= (posXNest+(float)(25.0f/SCR_WIDTH)) 
			 && _y >= (posYNest-(float)(25.0f/SCR_WIDTH))
			 && _y <= (posYNest+(float)(25.0f/SCR_WIDTH)))
			{
				_state = EXPLORER;
				_pheromoneType = 1;
				_placePheromoneIntensity = 60; // TODO
				_lifeTime = 0;

				if(_carryingFood == true)
				{
					_theta += glm::radians((float)(180.0f));
				}
				_carryingFood = false;	
			}

			else if(foodColision(foodSources))
			{
				_state = CARRIER;
				_pheromoneType = 2;
				_placePheromoneIntensity = 20;
				_lifeTime = 0;

				if(_carryingFood == false)
				{
					_theta += glm::radians((float)(180.0f));
				}
				_carryingFood = true;		
			}

		break;

		default:

		break;
	}
}
void Ant::putPheromone(int* pheromoneMatrix)
{

}

void Ant::draw()
{

}

void Ant::run()
{

}
	
