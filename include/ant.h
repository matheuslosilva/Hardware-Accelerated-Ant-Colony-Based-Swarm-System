#ifndef ANT_H
#define ANT_H

#include <constants.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <sinCosLookup.h>
#include <foodsource.h>
#include <antColony.h>

using namespace std;

enum States
{
	EXPLORER,
	BACKHOME,
	CARRIER,
	NESTCARRIER,
	PERFECTCARRIER,
	FOLLOWGREEN
};

class Ant 
{
	public:
		// Ant state
		float _x, _y;
		float _theta;
		float _size;

		float _xSensorR, _ySensorR;
		float _xSensorL, _ySensorL;
		float _xDistanceSensor;
		float _yDistanceSensor;
		int _sensorArea;
		
		
		float _velocity;
		int _placePheromoneIntensity;
		int   _pheromoneType;
		int _explorerPheromoneIntensity, _nestCarrierPheromoneIntensity, _carrierPheromoneIntensity;
		glm::vec3 _color;

		bool _foundNest, _foundFood;
		bool _carryingFood;

		States _state;
		int _lifeTime;
		

	public:
		Ant(float x, float y, float theta, float size, float velocity);
		Ant(float x, float y, float theta, float size, float velocity, int explorerPheromoneIntensity, int nestCarrierPheromoneIntensity, int carrierPheromoneIntensity);

		
		//int getChoice() const { return _choice; }
		//enum State { REST, SEARCH_NEW_NESTBOX, FIND_NESTBOX, BACK_TO_HOME, DANCE };
		//State getState() const { return _state; }
		void environmentAnalysis(int viewFrequency, vector<uint8_t> &pheromoneMatrix, vector<AntColony*> antColonies, vector<FoodSource*> foodSources);
		bool nestColision(vector<AntColony*> antColonies);
		bool foodColision(vector<FoodSource*> foodSources);
		void changeState(States newState);
		void makeDecision(vector<AntColony*> antColonies, vector<FoodSource*> foodSources,  int lR, int lG, int lB, int rR, int rG, int rB);
		void move(int l);
};
#endif