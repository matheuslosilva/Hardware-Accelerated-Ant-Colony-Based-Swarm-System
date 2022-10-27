#ifndef ANTCOLONY_H
#define ANTCOLONY_H

using namespace std;
#include <vector>
#include <constants.h>
class Ant;

class AntColony 
{
	public:
		int id;
		vector<Ant*> ants;
		int numberOfAnts;
		int food;

		float posX, posY;
		float size;

		AntColony(int numberAnts, float x, float y, float newSize, int id);
		AntColony(int numberAnts, float x, float y, float newSize, int id, int explorerPheromoneIntensity, int nestCarrierPheromoneIntensity, int carrierPheromoneIntensity, int sensorAngle);
		bool antColision(float antPosx, float antPosY);


};
#endif