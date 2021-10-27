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

using namespace std;

enum States
{
	EXPLORER,
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
		float _placePheromoneIntensity;
		int   _pheromoneType;
		glm::vec3 _color;

		bool _foundNest, _foundFood;
		bool _carryingFood;

		States _state;
		int _lifeTime;
		

	public:
		Ant(float x, float y, float theta, float size, float velocity, float placePheromoneIntensity);

		
		//int getChoice() const { return _choice; }
		//enum State { REST, SEARCH_NEW_NESTBOX, FIND_NESTBOX, BACK_TO_HOME, DANCE };
		//State getState() const { return _state; }
		void environmentAnalysis(int viewFrequency, vector<uint8_t> &pheromoneMatrix, float posXN, float posYN, vector<FoodSource*> foodSources);
		bool foodColision(vector<FoodSource*> foodSources);
		void changeState(float posXN, float posYN, vector<FoodSource*> foodSources,  int lR, int lG, int lB, int rR, int rG, int rB);
		void putPheromone(int* pheromoneMatrix);
		void move(int l);
		void draw();
		void run();
	

};
#endif