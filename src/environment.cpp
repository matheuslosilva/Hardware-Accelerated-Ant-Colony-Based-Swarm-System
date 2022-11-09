#include <environment.h>

Environment::Environment(int pheromoneMatrixUpdatePixelsFrameRate, int pheromoneEvaporationFrameRate)
{
	for(int i = 0; i < DATA_SIZE; i++) pheromoneMatrix[i] = 0;
	this->pheromoneMatrixUpdatePixelsFrameRate = pheromoneMatrixUpdatePixelsFrameRate;
	this->pheromoneEvaporationFrameRate = pheromoneEvaporationFrameRate;
}

void Environment::initializeEnvironment(OpenglBuffersManager* openglBuffersManager, int explorerPheromoneIntensity, int nestCarrierPheromoneIntensity, int carrierPheromoneIntensity, int leftSensorAngle, int rightSensorAngle)
{
	/*
	int numberOfAnts	=	400;
	float nestPosX 		=	-0.1f;
	float nestPosY 		=	0.0f;
	float nestSize 		=	10.0f; //this is equal 20X20 square of pixels
	float nestID 		=	0;

	int foodAmount		= 	0; // TODO
	float foodPosX 		=  	0.2f;
	float foodPosY  	=	0.0f;
	float foodSize 		=	10.0f;
	float foodID		=	0;

	Nests.push_back(new AntColony(numberOfAnts, nestPosX, nestPosY, nestSize, nestID));
	foodSources.push_back(new FoodSource(foodAmount, foodPosX, foodPosY, foodSize, foodID));
	*/

	Nests.push_back(new AntColony(100	, 0.0, 0.0, 10.0, 0, explorerPheromoneIntensity, nestCarrierPheromoneIntensity, carrierPheromoneIntensity, leftSensorAngle, rightSensorAngle));
	foodSources.push_back(new FoodSource(0, -0.25, -0.25, 10.0, 0));
	foodSources.push_back(new FoodSource(0, 0.25, -0.25, 10.0, 1));
	foodSources.push_back(new FoodSource(0, -0.25, 0.25, 10.0, 2));
	foodSources.push_back(new FoodSource(0, 0.25, 0.25, 10.0, 3));

	openglBuffersManager->createNestComponents(Nests[0]);

    openglBuffersManager->createAntsComponents(Nests[0]);

    openglBuffersManager->createFoodComponents(foodSources[0]);
    openglBuffersManager->createFoodComponents(foodSources[1]);
    openglBuffersManager->createFoodComponents(foodSources[2]);
    openglBuffersManager->createFoodComponents(foodSources[3]);

	numberOfNests = 1;
	numberOfFoods = 4;
}

void Environment::resetEnvironment()
{
	Nests.clear();
	foodSources.clear();
	numberOfNests = 0;
	numberOfFoods = 0;
	for(int i = 0; i < DATA_SIZE; i++) pheromoneMatrix[i] = 0;
}

void Environment::createNest(UI* userInterface, OpenglBuffersManager* openglBuffersManager)
{
	Nests.push_back(new AntColony(userInterface->nOfAnts, (float)userInterface->nestPosX/SCR_WIDTH, (float)userInterface->nestPosY/SCR_HEIGHT,10.0, numberOfNests));

	openglBuffersManager->createNestComponents(Nests[numberOfNests]);

    openglBuffersManager->createAntsComponents(Nests[numberOfNests]);

    numberOfNests++;
}

void Environment::createFoodSource(UI* userInterface, OpenglBuffersManager* openglBuffersManager)
{
	foodSources.push_back(new FoodSource(userInterface->foodAmount,(float) userInterface->foodPosX/SCR_WIDTH, (float)userInterface->foodPosY/SCR_HEIGHT,  10.0, numberOfFoods));

	openglBuffersManager->createFoodComponents(foodSources[numberOfFoods]);

	numberOfFoods++;
}

void Environment::drawNests(OpenglBuffersManager* openglBuffersManager)
{
	openglBuffersManager->drawNests(numberOfNests);

	for(int i = 0; i < numberOfNests; i++)
	{
		openglBuffersManager->updateModelAnts(Nests[i]);           

        openglBuffersManager->drawAnts(Nests[i]);
	}
}

void Environment::drawFoods(OpenglBuffersManager* openglBuffersManager)
{
	openglBuffersManager->drawFoods(numberOfFoods);  
}

void Environment::draw(OpenglBuffersManager* openglBuffersManager)
{
	drawFoods(openglBuffersManager);

	drawNests(openglBuffersManager);

	openglBuffersManager->drawPheromone(pheromoneMatrix);

	
}

void Environment::moveAnts(int frameCounter)
{
	for(int i = 0; i < numberOfNests; i++)
	{
		AntColony* actualNest = Nests[i];
		for(int i = 0; i < actualNest->numberOfAnts; i++)
	    {      
	        //cout<<endl; 
			actualNest->ants[i]->move(frameCounter);

	        actualNest->ants[i]->environmentAnalysis(frameCounter, pheromoneMatrix, Nests, foodSources);
	    }
	}
}

void Environment::updatePheromonePixels(int frameCounter)
{
	if(frameCounter % pheromoneMatrixUpdatePixelsFrameRate == 0)
    {
    	for(int j = 0; j < numberOfNests; j++)
    	{
		    for(int i = 0; i < Nests[j]->numberOfAnts; i++)
		    {
		    	Ant* currentAnt = Nests[j]->ants[i];

		        int xn, yn, index;
		      
		        xn = ((SCR_WIDTH/2) + currentAnt->_x * (SCR_WIDTH/2));
		        yn = ((SCR_HEIGHT/2) + currentAnt->_y * (SCR_HEIGHT/2));
		        index = (yn * SCR_WIDTH) + xn;

		        if(currentAnt->_pheromoneType == 1)
		        {
		            pheromoneMatrix[index*4] = min((int)(pheromoneMatrix[index*4] + currentAnt->_placePheromoneIntensity), 255);
		        }
		        else if(currentAnt->_pheromoneType == 2)
		        {
		            pheromoneMatrix[(index*4)+1] = min((int)(pheromoneMatrix[(index*4)+1] + currentAnt->_placePheromoneIntensity), 255);
		        }
		        else if(currentAnt->_pheromoneType == 3)
		        {
		            pheromoneMatrix[(index*4)+2] = min((int)(pheromoneMatrix[(index*4)+2] + currentAnt->_placePheromoneIntensity), 255);
		        }


		    } 
		}
	}
}

void Environment::pheromoneEvaporation(int frameCounter)
{
	if(frameCounter % pheromoneEvaporationFrameRate == 0)
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