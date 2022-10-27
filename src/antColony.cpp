#include <antColony.h>
#include <ant.h>

AntColony::AntColony(int numberAnts, float x, float y, float newSize, int newid)
{
    id = newid;
    numberOfAnts = numberAnts;
    posX = x;
    posY = y;
    size = newSize;
    food = 0;
    
	for(int i = 0; i < numberOfAnts; i++)
    {	
        //ants.push_back(new Ant((rand()%100-50)/1000.0f, (rand()%100-50)/1000.0f, glm::radians((float)(rand()%360)), 0.02f, 0.00001f, 10.0f, 1));					
                          //          x                                  y                      theta                                  resize velocity  pheromoneIntensity
        ants.push_back(new Ant(posX, posY, glm::radians((float)(rand()%360)), 0.10f, 0.0002f));
        
        //if(i > POP_SIZE/2) ants.push_back(new Ant(0.98f, 0.98f, glm::radians((float)(rand()%360)), 0.02f, 0.00004f, 10.0f, 1));
        //else ants.push_back(new Ant(-0.98f, -0.98f, glm::radians((float)(rand()%360)), 0.02f, 0.00004f, 10.0f, 2));
    }
}

AntColony::AntColony(int numberAnts, float x, float y, float newSize, int newid, int explorerPheromoneIntensity, int nestCarrierPheromoneIntensity, int carrierPheromoneIntensity, int sensorAngle)
{
    id = newid;
    numberOfAnts = numberAnts;
    posX = x;
    posY = y;
    size = newSize;
    food = 0;

	for(int i = 0; i < numberOfAnts; i++)
    {	
        //ants.push_back(new Ant((rand()%100-50)/1000.0f, (rand()%100-50)/1000.0f, glm::radians((float)(rand()%360)), 0.02f, 0.00001f, 10.0f, 1));					
                          //          x                                  y                      theta                                  resize velocity  pheromoneIntensity
        ants.push_back(new Ant(posX, posY, glm::radians((float)(rand()%360)), 0.10f, 0.0002f, explorerPheromoneIntensity, nestCarrierPheromoneIntensity, carrierPheromoneIntensity, sensorAngle));
        
        //if(i > POP_SIZE/2) ants.push_back(new Ant(0.98f, 0.98f, glm::radians((float)(rand()%360)), 0.02f, 0.00004f, 10.0f, 1));
        //else ants.push_back(new Ant(-0.98f, -0.98f, glm::radians((float)(rand()%360)), 0.02f, 0.00004f, 10.0f, 2));
    }
}

bool AntColony::antColision(float antPosx, float antPosY)
{  
    float range = (size / SCR_HEIGHT) * 2;

	if(    antPosx >= (posX-range) 
        && antPosx <= (posX+range) 
        && antPosY >= (posY-range)
        && antPosY <= (posY+range))
        return true;
    else
        return false;
}