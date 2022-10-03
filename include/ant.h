#ifndef ANT_H
#define ANT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <sinCosLookup.h>

#include <foodSource.h>
#include <anthill.h>
#include <antSensor.h>

#include <parameterAssigner.h>

using namespace std;

class Ant 
{
	public:
		// Ant state
		int id;

		int nestID;
		float posX;
		float posY;
		float size;
		float theta;
		float velocity; 

		AntStates state;
		int pheromoneType;
		int placePheromoneIntensity;

		int lifeTime;
		int viewFrequency;

		AntSensor* pheromoneSensorR;
		AntSensor* pheromoneSensorL;

		bool foundNest;
		bool foundFood;
		bool carryingFood;

	public:
		Ant(int id, float posX, float posY, AntParameters* antParameters);

		void environmentAnalysis(int viewFrequency, uint8_t* pheromoneMatrix, vector<Anthill*> antColonies, vector<FoodSource*> foodSources);
		bool nestColision(vector<Anthill*> antColonies);
		bool foodColision(vector<FoodSource*> foodSources);
		void changeState(AntStates newState);
		void makeDecision(vector<Anthill*> antColonies, vector<FoodSource*> foodSources,  int lR, int lG, int lB, int rR, int rG, int rB);
		void move(int l);
};
#endif