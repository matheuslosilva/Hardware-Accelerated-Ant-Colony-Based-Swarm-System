#ifndef ANTHILL_H
#define ANTHILL_H

#include <vector>
#include <parameterAssigner.h>

using namespace std;

class Anthill 
{
	public:
		int id;

		float posX;
		float posY;
		float size;
		int antAmount;
		int foodAmount;


		Anthill(AnthillParameters* anthillParameters);
		bool antColision(float antPosx, float antPosY);


};
#endif