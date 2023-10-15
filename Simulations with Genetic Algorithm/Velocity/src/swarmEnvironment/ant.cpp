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
Ant::Ant(int newId, float newPosX, float newPosY, AntParameters* antParameters)
{
	id = newId;

	posX = newPosX;
	posY = newPosY;
	nestID = antParameters->nestID;
	size = antParameters->size;
	theta = glm::radians((float)(rand()%360));
	velocity = antParameters->velocity;

	state = antParameters->state;
	pheromoneType = 1;
	placePheromoneIntensity = antParameters->placePheromoneIntensity;

	lifeTime = 0;
	viewFrequency = antParameters->viewFrequency;

	pheromoneSensorR = new AntSensor(0, antParameters->antSensorParameters);
	pheromoneSensorL = new AntSensor(1, antParameters->antSensorParameters2);

	carryingFood = false;
}

void Ant::move(int l)
{
	lifeTime++;
	int angle = (int)((theta/M_PI)*1800);
	if(angle < 0) angle += 3600;
	if(angle >= 3600) angle -= 3600;

	posX+= velocity*cosLookup[angle];
	posY+= velocity*sinLookup[angle];
	
	//Border treatment
	if(posX< -0.990f || posX> 0.990f) 
	{
		posX= posX< 0 ? -0.990f : 0.990f;	
	} 

	if(posY< -0.990f || posY> 0.990f) 
	{
		posY= posY< 0 ? -0.990f : 0.990f;
	}

}

void Ant::environmentAnalysis(int frameCounter, uint8_t* pheromoneMatrix, vector<Anthill*> antColonies, vector<FoodSource*> foodSources)
{
	if(frameCounter % viewFrequency== 0)
	{
			
		pheromoneSensorL->move(posX, posY, theta);
		pheromoneSensorR->move(posX, posY, theta);

		makeDecision(antColonies, foodSources, 
			pheromoneSensorL->detectPheromone(pheromoneMatrix, RED), pheromoneSensorL->detectPheromone(pheromoneMatrix, GREEN), pheromoneSensorL->detectPheromone(pheromoneMatrix, BLUE), 
			pheromoneSensorR->detectPheromone(pheromoneMatrix, RED), pheromoneSensorR->detectPheromone(pheromoneMatrix, GREEN), pheromoneSensorR->detectPheromone(pheromoneMatrix, BLUE));
		
		// Border Treatment
		//if(xSensorL < -0.990f || xSensorL > 0.990f || ySensorL < -0.990f || ySensorL > 0.990f) theta += glm::radians((float)(rand()%360)/10.0f-1.0f)*4.0f;
		//else if(xSensorR < -0.990f || xSensorR > 0.990f || ySensorR < -0.990f || ySensorR > 0.990f) theta -= glm::radians((float)(rand()%360)/10.0f-1.0f)*4.0f;

		if (theta < 0) theta += 2*M_PI;
		if (theta >= 2*M_PI) theta -= 2*M_PI;
	}		
}

bool Ant::foodColision(vector<FoodSource*> foodSources)
{
	for(int i = 0; i < (int)foodSources.size(); i++)
	{
		if(foodSources[i]->antColision(posX, posY))
		{
			posX = foodSources[i]->posX;
			posY = foodSources[i]->posY;
			carryingFood = true;	
			return true;
		}
	}
	return false;
}

bool Ant::nestColision(vector<Anthill*> antColonies)
{
	for(int i = 0; i < (int)antColonies.size(); i++)
	{
		if(antColonies[i]->antColision(posX, posY))
		{
			if(carryingFood) {
				antColonies[i]->foodAmount++;
			}
			posX = antColonies[i]->posX;
			posY = antColonies[i]->posY;
			carryingFood = false;
			return true;
		}
	}
	return false;
}

void Ant::changeState(AntStates newState)
{
	switch(newState)
	{
		case EXPLORER:

			state = EXPLORER;
			pheromoneType = 1;
			placePheromoneIntensity = 60;
			 	
		break;

		case BACKHOME:

			theta += glm::radians((float)(180.0f));
			state = BACKHOME;
			pheromoneType = -1;
			 	
		break;

		case CARRIER:

			state = CARRIER;
			pheromoneType = 2;
			placePheromoneIntensity = 60;

		break;

		case NESTCARRIER:

			state = NESTCARRIER;
			pheromoneType = 2;
			placePheromoneIntensity = 60;		

		break;

		case FOLLOWGREEN:

			state = FOLLOWGREEN;
			pheromoneType = -1;

		break;

		default:
		break;
	}
}

void Ant::makeDecision(vector<Anthill*> antColonies, vector < FoodSource* > foodSources, int lR, int lG, int lB, int rR, int rG, int rB)
{
	switch(state)
	{
		case EXPLORER:
	
			if(rR > lR)
				theta += glm::radians((float)(rand()%360)/6.0f)*0.1f;
			else  if(rR < lR)
				theta -= glm::radians((float)(rand()%360)/6.0f)*0.1f;

			if(rG > 0 || lG > 0)
			{
				changeState(FOLLOWGREEN);
			}

			if(foodColision(foodSources))
			{
				theta += glm::radians((float)(180.0f));	
				lifeTime = 0;

				changeState(CARRIER);
			}
		
			 	
		break;

		case BACKHOME:

			if(rR > lR)
				theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else  if(rR < lR)
				theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			
			if(nestColision(antColonies))
			{			
				theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
				theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
				lifeTime = 0;

				changeState(EXPLORER);
			}
			 	
		break;

		case CARRIER:

			if(rG > lG)
				theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rG < lG)
				theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;

			if(rR > lR)
				theta -= glm::radians((float)(rand()%360)/6.0f)*0.1f;
			else if(rR < lR)
				theta += glm::radians((float)(rand()%360)/6.0f)*0.1f;

			if(nestColision(antColonies))
			{				
				theta += glm::radians((float)(180.0f));
				lifeTime = 0;
				
				changeState(NESTCARRIER);
			}
			break;

		case NESTCARRIER:

			if(rG  > lG)
				theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rG < lG)
				theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			
			if(carryingFood == true && nestColision(antColonies))
			{
				lifeTime = 0;	
				theta += glm::radians((float)(180.0f));
				placePheromoneIntensity = 60;	
			}

			else if(carryingFood == false && foodColision(foodSources))
			{
				
				theta += glm::radians((float)(180.0f));
				placePheromoneIntensity = 60;
			}
			break;

		case FOLLOWGREEN:
			if(rG  > lG)
				theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rG < lG)
				theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			
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

	if(lifeTime%10 == 0) placePheromoneIntensity = max(0, placePheromoneIntensity - 3);
	if(lifeTime >= 200)
	{
		lifeTime = 0;
		posX = 0;
		posY = 0;
		changeState(NESTCARRIER);
	}
}

	
