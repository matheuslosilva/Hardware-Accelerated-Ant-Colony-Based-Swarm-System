#include <foodsource.h>
#include <constants.h>

FoodSource::FoodSource(int food, float x, float y, float newSize, int newId)
{
    foodAmount = food;
    posX = x;
    posY = y;
    size = newSize;
    id = newId;
}

bool FoodSource::antColision(float antPosx, float antPosY)
{
    float range = (size / SCR_HEIGHT) * 2;
    
    if(    antPosx >= (posX-range) 
        && antPosx <= (posX+range) 
        && antPosY >= (posY-range)
        && antPosY <= (posY+range))
        return true;
    else
        return false;      
}