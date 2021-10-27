#include <foodsource.h>
#include <constants.h>

FoodSource::FoodSource(int food, float x, float y, int newId)
{
    foodAmount = food;
    posX = x;
    posY = y;
    id = newId;
}

bool FoodSource::antColision(float antPosx, float antPosY)
{
    if(    antPosx >= (posX-(float)(25.0f/SCR_WIDTH)) 
        && antPosx <= (posX+(float)(25.0f/SCR_WIDTH)) 
        && antPosY >= (posY-(float)(25.0f/SCR_WIDTH))
        && antPosY <= (posY+(float)(25.0f/SCR_WIDTH)))
        return true;
    else
        return false;      
}