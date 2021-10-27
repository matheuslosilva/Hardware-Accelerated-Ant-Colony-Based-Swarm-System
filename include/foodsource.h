#ifndef FOODSOURCE_H
#define FOODSOURCE_H

class Ant;

using namespace std;
	
class FoodSource 
{
	public:
		int id;
		int foodAmount;

		float posX, posY;

		FoodSource(int food, float x, float y, int id);

		bool antColision(float antPosx, float antPosY);


};
#endif