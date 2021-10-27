#include <antColony.h>

AntColony::AntColony(int numberAnts, float x, float y, int newid)
{
    id = newid;
    numberOfAnts = numberAnts;
    if(numberOfAnts == 0) numberOfAnts = 200;
    posX = x;
    posY = y;

    
	for(int i = 0; i < numberOfAnts; i++)
    {	
        //ants.push_back(new Ant((rand()%100-50)/1000.0f, (rand()%100-50)/1000.0f, glm::radians((float)(rand()%360)), 0.02f, 0.00001f, 10.0f, 1));					
                          //          x                                  y                      theta                                  resize velocity  pheromoneIntensity
        ants.push_back(new Ant(posX, posY, glm::radians((float)(rand()%360)), 0.10f, 0.0002f, 5.0f));
        
        //if(i > POP_SIZE/2) ants.push_back(new Ant(0.98f, 0.98f, glm::radians((float)(rand()%360)), 0.02f, 0.00004f, 10.0f, 1));
        //else ants.push_back(new Ant(-0.98f, -0.98f, glm::radians((float)(rand()%360)), 0.02f, 0.00004f, 10.0f, 2));
    }
}

void AntColony::moveAnts(int frameCounter, vector<uint8_t> &pheromoneMatrix, vector<FoodSource*> foodSources)
{
	for(int i = 0; i < numberOfAnts; i++)
    {      
        //cout<<endl; 
		ants[i]->move(frameCounter);

        ants[i]->environmentAnalysis(frameCounter, pheromoneMatrix, posX, posY, foodSources);

    }
}