#ifndef ANT_H
#define ANT_H

#include <constants.h>

using namespace std;

class Ant 
{
	public:
		// Ant state
		float _x, _y;
		float _xSensorR, _ySensorR;
		float _xSensorL, _ySensorL;
		float _theta;
		float _size;
		float _velocity;
		float _placePheromoneIntensity;
		int   _pheromoneType;
		glm::vec3 _color;

	public:
		Ant(float x, float y, float theta, float size, float velocity, float placePheromoneIntensity, int pheromoneType);

		
		//int getChoice() const { return _choice; }
		//enum State { REST, SEARCH_NEW_NESTBOX, FIND_NESTBOX, BACK_TO_HOME, DANCE };
		//State getState() const { return _state; }
		void environmentAnalysis(int viewFrequency, vector<uint8_t> &pheromoneMatrix);
		void putPheromone(int* pheromoneMatrix);
		void move(int l);
		void draw();
		void run();
	

};
#endif