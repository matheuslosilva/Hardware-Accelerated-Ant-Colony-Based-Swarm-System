#include <environment.h>
#include <iostream>

Environment::Environment(ParameterAssigner* parameterAssigner)
{
	this->parameterAssigner = parameterAssigner;

	pheromoneMatrix = (uint8_t*)malloc(sizeof(uint8_t) * DATA_SIZE);
}

void Environment::initializeEnvironment(OpenglBuffersManager* openglBuffersManager)
{
    for(int i = 0; i < DATA_SIZE; i+=4) pheromoneMatrix[i] = 0; //R
    for(int i = 1; i < DATA_SIZE; i+=4) pheromoneMatrix[i] = 0; //G
    for(int i = 2; i < DATA_SIZE; i+=4) pheromoneMatrix[i] = 0; //B   
    for(int i = 3; i < DATA_SIZE; i+=4) pheromoneMatrix[i] = 255; //A

	numberOfNests = 0;
	numberOfFoods = 0;
	numberOfAnts = 0;

	placePheromoneRate = parameterAssigner->environmentParameters.placePheromoneRate;
	pheromoneEvaporationRate = parameterAssigner->environmentParameters.pheromoneEvaporationRate;
}

void Environment::resetEnvironment()
{
	nests.clear();
	foods.clear();
	ants.clear();
	numberOfNests = 0;
	numberOfFoods = 0;
	numberOfAnts = 0;
	for(int i = 0; i < DATA_SIZE; i+=4) pheromoneMatrix[i] = 0; //R
    for(int i = 1; i < DATA_SIZE; i+=4) pheromoneMatrix[i] = 0; //G
    for(int i = 2; i < DATA_SIZE; i+=4) pheromoneMatrix[i] = 0; //B   
    for(int i = 3; i < DATA_SIZE; i+=4) pheromoneMatrix[i] = 255; //A

    placePheromoneRate = 1;
	pheromoneEvaporationRate = 1;
}

void Environment::createNest(int idNest, OpenglBuffersManager* openglBuffersManager)
{  
	AnthillParameters* anthillParameters = parameterAssigner->anthillParameters[idNest];
    
    Anthill* anthill = new Anthill(anthillParameters);
    nests.push_back(anthill);
    
    numberOfNests++;

    openglBuffersManager->addElement(&(openglBuffersManager->anthillsTransformationMatrices), anthill->size, anthill->posX, anthill->posY, 0.0f);
    openglBuffersManager->updateBuffer(openglBuffersManager->anthillsTransformationMatricesVBO, numberOfNests, openglBuffersManager->anthillsTransformationMatrices, GL_STATIC_DRAW);
}

void Environment::createFoodSource(int idFood, OpenglBuffersManager* openglBuffersManager)
{
	FoodSourceParameters* foodParameters = parameterAssigner->foodParameters[idFood];

	FoodSource* food = new FoodSource(foodParameters);
    foods.push_back(food);
    
    numberOfFoods++; 

    openglBuffersManager->addElement(&(openglBuffersManager->foodsTransformationMatrices), food->size, food->posX, food->posY, 0.0f);
    openglBuffersManager->updateBuffer(openglBuffersManager->foodsTransformationMatricesVBO, numberOfFoods, openglBuffersManager->foodsTransformationMatrices, GL_STATIC_DRAW);
}

void Environment::createAnt(int idNest, OpenglBuffersManager* openglBuffersManager)
{
	int antEspecificationIndex = parameterAssigner->anthillParameters[idNest]->antEspecification;
	
	int antAmount = parameterAssigner->anthillParameters[idNest]->antAmount;
	float posX = parameterAssigner->anthillParameters[idNest]->posX;
	float posY = parameterAssigner->anthillParameters[idNest]->posY;

	AntParameters* antParameters = parameterAssigner->antParameters[antEspecificationIndex];

	for(int i = 0; i < antAmount; i++)
	{
	 	Ant* ant = new Ant(numberOfAnts, posX, posY, antParameters);
	    ants.push_back(ant);
	    
	    numberOfAnts++;

	    openglBuffersManager->addElement(&(openglBuffersManager->antsTransformationMatrices), ant->size, ant->posX, ant->posY, ant->theta);
	}
	openglBuffersManager->updateBuffer(openglBuffersManager->antsTransformationMatricesVBO, numberOfAnts, openglBuffersManager->antsTransformationMatrices, GL_DYNAMIC_DRAW);
}

void Environment::run(int frameCounter)
{
	moveAnts(frameCounter); // TODO CUDA

	placePheromone(frameCounter); 

	pheromoneEvaporation(frameCounter);  
}

void Environment::draw(OpenglBuffersManager* openglBuffersManager, Camera* camera)
{
	openglBuffersManager->updateModelAnts(numberOfAnts, ants);
	openglBuffersManager->drawAnts(numberOfAnts, camera);  
	openglBuffersManager->drawAnthills(numberOfNests, camera);
	openglBuffersManager->drawFoods(numberOfFoods, camera);
	openglBuffersManager->drawPheromone(pheromoneMatrix, camera); 
}

void Environment::moveAnts(int frameCounter)
{
	for (int i = 0; i < numberOfAnts; i++)
    {      
		ants[i]->move(frameCounter);
        ants[i]->environmentAnalysis(frameCounter, pheromoneMatrix, nests, foods);
    }
}

void Environment::placePheromone(int frameCounter)
{
	if (frameCounter % placePheromoneRate == 0)
    {
	    for (int i = 0; i < numberOfAnts; i++)
	    {
	    	Ant* currentAnt = ants[i];

	        int xn, yn, index;
	      
	        xn = ((PIXEL_WIDTH/2) + currentAnt->posX * (PIXEL_WIDTH/2));
	        yn = ((PIXEL_HEIGHT/2) + currentAnt->posY * (PIXEL_HEIGHT/2));
	        index = (yn * PIXEL_WIDTH) + xn;

	        if (currentAnt->pheromoneType == 1)
	        {	        	
	            pheromoneMatrix[index*4] = min((int)(pheromoneMatrix[index*4] + currentAnt->placePheromoneIntensity), 255);
	        }
	        else if (currentAnt->pheromoneType == 2)
	        {
	            pheromoneMatrix[(index*4)+1] = min((int)(pheromoneMatrix[(index*4)+1] + currentAnt->placePheromoneIntensity), 255);
	        }
	        else if (currentAnt->pheromoneType == 3)
	        {
	            pheromoneMatrix[(index*4)+2] = min((int)(pheromoneMatrix[(index*4)+2] + currentAnt->placePheromoneIntensity), 255);
	        }
		}
	}
}

void Environment::pheromoneEvaporation(int frameCounter)
{
	if (frameCounter % pheromoneEvaporationRate == 0)
    {
	    for(int i = 0; i < DATA_SIZE; i+=4)
	    {
	        pheromoneMatrix[i] = max((int)pheromoneMatrix[i]-1, 0);
	    }
	    for(int i = 1; i < DATA_SIZE; i+=4)
	    {
	        pheromoneMatrix[i] = max((int)pheromoneMatrix[i]-1, 0);
	    }
	    for(int i = 2; i < DATA_SIZE; i+=4)
	    {
	        pheromoneMatrix[i] = max((int)pheromoneMatrix[i]-1, 0);
	    }
	}
}