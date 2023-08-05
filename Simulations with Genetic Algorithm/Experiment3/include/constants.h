#ifndef CONSTANTS_H
#define CONSTANTS_H

// Environment
extern unsigned int PIXEL_WIDTH;
extern unsigned int PIXEL_HEIGHT;
extern unsigned int GLOBAL_SEED;

extern float VELOCITY;
extern unsigned int RED_PHEROMONE_INTENSITY;
extern unsigned int GREEN_PHEROMONE_INTENSITY;
extern int TIMEOUT;
extern int PHEROMONE_EVAPORATION;
extern int CHANGE_TO_EXPLORER;
extern int CHANGE_TO_CARRIER;
extern int CHANGE_TO_NESTCARRIER;
extern int CHANGE_TO_FOLLOWGREEN;

void setGlobalSeed(unsigned int globalSeed);
void setScrHeight(unsigned int scrHeight);
void setScrWidth(unsigned int scrWidth);
#endif