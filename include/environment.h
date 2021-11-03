#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <bitset>

#include <UI.h>
#include <openglBuffersManager.h>
#include <antColony.h>

#include <constants.h>

#include <foodsource.h>

#include <vector>

class Environment
{
	public:

		vector<uint8_t> pheromoneMatrix = vector<uint8_t>(DATA_SIZE);
		
		int pheromoneMatrixUpdatePixelsFrameRate;
		int pheromoneEvaporationFrameRate;

		int numberOfNests;
		int numberOfFoods;

		vector<AntColony*> Nests;
		vector<FoodSource*> foodSources;

	public:

		Environment(int pheromoneMatrixUpdatePixelsFrameRate, int pheromoneEvaporationFrameRate, OpenglBuffersManager* openglBuffersManager);

		void resetEnvironment();

		void createNest(UI* userInterface, OpenglBuffersManager* openglBuffersManager);
		void createFoodSource(UI* userInterface, OpenglBuffersManager* openglBuffersManager);

		void drawNests(OpenglBuffersManager* openglBuffersManager);
		void drawFoods(OpenglBuffersManager* openglBuffersManager);
		void draw(OpenglBuffersManager* openglBuffersManager);

		void moveAnts(int frameCounter);

		void updatePheromonePixels(int frameCounter);

		void pheromoneEvaporation(int frameCounter);






};






#endif