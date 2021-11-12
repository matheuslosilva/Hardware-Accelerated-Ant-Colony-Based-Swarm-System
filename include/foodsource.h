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
		float size;

		FoodSource(int food, float x, float y, float newSize, int id);

		bool antColision(float antPosx, float antPosY);


};
#endif