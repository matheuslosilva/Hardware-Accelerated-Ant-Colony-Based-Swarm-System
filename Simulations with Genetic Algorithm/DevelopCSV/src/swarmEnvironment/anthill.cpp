#include <anthill.h>
#include <iostream>
Anthill::Anthill(AnthillParameters* anthillParameters)
{
    id = anthillParameters->id;
    
    posX = anthillParameters->posX;
    posY = anthillParameters->posY;
    size = anthillParameters->size;
    antAmount = anthillParameters->antAmount;
    foodAmount = 0;
}

bool Anthill::antColision(float antPosx, float antPosY)
{  
	if(    antPosx >= (posX-size) 
        && antPosx <= (posX+size) 
        && antPosY >= (posY-size)
        && antPosY <= (posY+size))
        return true;
    else
        return false;
}