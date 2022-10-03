#ifndef PARAMETERASSIGNER_H
#define PARAMETERASSIGNER_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <constants.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class AntSensor;

using namespace rapidjson;
using namespace std;

enum PheromoneType
{
	RED,
	GREEN,
	BLUE
};

enum AntStates
{
	EXPLORER,
	BACKHOME,
	CARRIER,
	NESTCARRIER,
	FOLLOWGREEN
};

enum SensorType
{
	PHEROMONE,
	NEST,
	FOOD,
	OBSTACLE,
	ANT
};

typedef struct
{
	int placePheromoneRate;
   	int pheromoneEvaporationRate;
}EnvironmentParameters;

typedef struct 
{
	int id;
	float posX;
    float posY;
    float size;
    int antAmount;
    int antEspecification;
}AnthillParameters;

typedef struct
{
	int id;
	float posX;
	float posY;
	float size;
	int foodAmount;
}FoodSourceParameters;

typedef struct
{
	float xCenterAntDistance;
	float yCenterAntDistance;
	int positionAngle;
	int sensorPixelRadius;

	SensorType sensorType;
}AntSensorParameters;

typedef struct
{
	int nestID;
	float size;
	float velocity; 

	AntStates state;
	int pheromoneType;
	int placePheromoneIntensity;

	int lifeTime;
	int viewFrequency;

	vector<AntSensorParameters*> antSensorParametersvec;

	AntSensorParameters* antSensorParameters;
	AntSensorParameters* antSensorParameters2;

}AntParameters;

class ParameterAssigner 
{
	public:
		EnvironmentParameters environmentParameters;
		vector <AnthillParameters *> anthillParameters;
		vector <FoodSourceParameters *> foodParameters;
		vector <AntParameters *> antParameters;
		

	public:
		ParameterAssigner(const char* filePath);

		EnvironmentParameters setEnvironment();
		EnvironmentParameters getEnvironment();

		AnthillParameters setAnthill();
		AnthillParameters getAnthill(int id);

		FoodSourceParameters setFood();
		FoodSourceParameters getFood(int id);

		AntParameters setAnt();
		AntParameters getAnt(int id);

		AntSensorParameters setAntSensor();
		AntSensorParameters getAntSensor(int id);



};
#endif