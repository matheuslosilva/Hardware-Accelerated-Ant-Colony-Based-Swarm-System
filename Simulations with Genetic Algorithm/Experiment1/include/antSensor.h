#ifndef SENSOR_H
#define SENSOR_H

#include <parameterAssigner.h>
#include <sinCosLookup.h>
#include <constants.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class AntSensor 
{
	public:
		int id;

		float posX;
		float posY;
		float xCenterAntDistance;
		float yCenterAntDistance;
		float positionAngle;
		int sensorPixelRadius;

		int indexSensorX;
		int indexSensorY;

		SensorType sensorType;

		AntSensor(int id, AntSensorParameters* antSensorParameters);
		int detectPheromone(uint8_t* pheromoneMatrix, PheromoneType pheromoneType);
		void move(float antPosX, float antPosy, float theta);
};
#endif