#ifndef ANTCOLONY_H
#define ANTCOLONY_H

#include<constants.h>

using namespace std;
	
class AntColony 
{
	public:
		vector<Ant*> ants;
		glm::mat4* antsModelMatrices;

		AntColony();

		glm::mat4* createAntsModelMatrices(vector<Ant*> antsColony);
		void updateModelAnts();
		void moveAnts(int l, vector<uint8_t> &pheromoneMatrix);


};
#endif