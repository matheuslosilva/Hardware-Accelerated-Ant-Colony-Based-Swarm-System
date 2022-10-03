#ifndef FOODSOURCE_H
#define FOODSOURCE_H

#include <constants.h>
#include <parameterAssigner.h>

using namespace std;
	
class FoodSource 
{
	public:
		int id;
		int foodAmount;
		float posX, posY;
		float size;

		FoodSource(FoodSourceParameters* foodParameters);

		bool antColision(float antPosx, float antPosY);
};
#endif