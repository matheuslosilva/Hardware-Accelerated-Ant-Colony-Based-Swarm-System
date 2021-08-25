#ifndef ANTCOLONY_H
#define ANTCOLONY_H

#include <environment.h>

using namespace std;
	
class AntColony 
{
	public:
		vector<Ant*> ants;
		glm::mat4* antsModelMatrices;

		AntColony();

		glm::mat4* createAntsModelMatrices(vector<Ant*> antsColony);
		void updateModelAnts();
		void moveAnts(int l);


};
#endif