#ifndef ANTCOLONY_H
#define ANTCOLONY_H

#include <ant.h>
#include <foodsource.h>
using namespace std;
	
class AntColony 
{
	public:
		int id;
		vector<Ant*> ants;
		int numberOfAnts;

		float posX, posY;

		AntColony(int numberAnts, float x, float y, int id);

		glm::mat4* createAntsModelMatrices(vector<Ant*> antsColony);
		void updateModelAnts();
		void moveAnts(int l, vector<uint8_t> &pheromoneMatrix, vector<FoodSource*> foodSources);


};
#endif