#include <antSensor.h>

AntSensor::AntSensor(int newId, AntSensorParameters* antSensorParameters)
{
	id = newId;
	posX = 0.0;
	posY = 0.0;

	xCenterAntDistance = antSensorParameters->xCenterAntDistance;
	yCenterAntDistance = antSensorParameters->yCenterAntDistance;
	positionAngle = glm::radians((float)antSensorParameters->positionAngle);

	sensorPixelRadius = antSensorParameters->sensorPixelRadius;
	sensorType = antSensorParameters->sensorType;
}	

void AntSensor::move(float antPosX, float antPosY, float theta)
{
	int angle = (int)(((theta+positionAngle)/M_PI)*1800);
	if(angle < 0) angle += 3600;
	if(angle >= 3600) angle -= 3600;

	posX = antPosX + xCenterAntDistance*cosLookup[angle]; 
	posY = antPosY + yCenterAntDistance*sinLookup[angle];

	indexSensorX = ((PIXEL_WIDTH/2) + posX * (PIXEL_WIDTH/2));
	indexSensorY = ((PIXEL_HEIGHT/2) + posY * (PIXEL_HEIGHT/2));
}

int AntSensor::detectPheromone(uint8_t* pheromoneMatrix, PheromoneType pheromoneType)
{
	int pheromoneDetected = 0;
	
	int index;

	for(int i = -sensorPixelRadius; i <= sensorPixelRadius; i++)
	{
		for(int j = -sensorPixelRadius; j <= sensorPixelRadius; j++)
		{
			index = ((indexSensorY+i) * PIXEL_WIDTH + (indexSensorX+j)) * 4;
			pheromoneDetected += pheromoneMatrix[index+pheromoneType];
		}
	}

	return pheromoneDetected;
}