#include <foodSource.h>
#include <iostream>

FoodSource::FoodSource(FoodSourceParameters* foodParameters)
{
    id = foodParameters->id;
    posX = foodParameters->posX;
    posY = foodParameters->posY;
    size = foodParameters->size;
    foodAmount = foodParameters->foodAmount;
}

bool FoodSource::antColision(float antPosx, float antPosY)
{
    if(    antPosx >= (posX-size) 
        && antPosx <= (posX+size) 
        && antPosY >= (posY-size)
        && antPosY <= (posY+size))
        return true;
    else
        return false;      
}