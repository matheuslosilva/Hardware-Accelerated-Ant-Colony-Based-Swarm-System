#include <environment.h>

Environment::Environment(int pheromoneMatrixUpdatePixelsFrameRate, int pheromoneEvaporationFrameRate,  OpenglBuffersManager* openglBuffersManager)
{
	for(int i = 0; i < DATA_SIZE; i++) pheromoneMatrix[i] = 0;
	this->pheromoneMatrixUpdatePixelsFrameRate = pheromoneMatrixUpdatePixelsFrameRate;
	this->pheromoneEvaporationFrameRate = pheromoneEvaporationFrameRate;

	Nests.push_back(new AntColony(400, -0.1f , 0.0f, 0));
	foodSources.push_back(new FoodSource(0, 0.3f , 0.0f, 0));

	openglBuffersManager->createNestComponents(Nests[0]);

    openglBuffersManager->createAntsComponents(Nests[0]);

    openglBuffersManager->createFoodComponents(foodSources[0]);

	numberOfNests = 1;
	numberOfFoods = 1;
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
	if(userInterface->nestPosX == 0 && userInterface->nestPosY == 0)	
		Nests.push_back(new AntColony(userInterface->nOfAnts, -0.1f , (float)userInterface->nestPosY, numberOfNests));
	else
		Nests.push_back(new AntColony(userInterface->nOfAnts, (float)userInterface->nestPosX/SCR_WIDTH, (float)userInterface->nestPosY/SCR_HEIGHT, numberOfNests));

	openglBuffersManager->createNestComponents(Nests[numberOfNests]);

    openglBuffersManager->createAntsComponents(Nests[numberOfNests]);

    numberOfNests++;
}

void Environment::createFoodSource(UI* userInterface, OpenglBuffersManager* openglBuffersManager)
{
	if(userInterface->foodPosX == 0 && userInterface->foodPosY == 0)	
		foodSources.push_back(new FoodSource(userInterface->foodAmount, 0.3f , (float)userInterface->foodPosY, numberOfFoods));
	else
		foodSources.push_back(new FoodSource(userInterface->foodAmount,(float) userInterface->foodPosX/SCR_WIDTH, (float)userInterface->foodPosY/SCR_HEIGHT, numberOfFoods));

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
		Nests[i]->moveAnts(frameCounter, pheromoneMatrix, foodSources);
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