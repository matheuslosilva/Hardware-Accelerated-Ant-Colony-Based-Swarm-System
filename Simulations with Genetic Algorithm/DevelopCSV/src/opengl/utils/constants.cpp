#include <constants.h>

unsigned int PIXEL_WIDTH = 2000;
unsigned int PIXEL_HEIGHT = 2000;
unsigned int GLOBAL_SEED = 0;

float VELOCITY = 0.0004;
unsigned int RED_PHEROMONE_INTENSITY = 60;
unsigned int GREEN_PHEROMONE_INTENSITY = 60;
int TIMEOUT = 400;
int PHEROMONE_EVAPORATION = 15;

// Environment
void setScrWidth(unsigned int scrWidth) {PIXEL_WIDTH  = scrWidth;}
void setScrHeight(unsigned int scrHeight) {PIXEL_HEIGHT = scrHeight;}
void setGlobalSeed(unsigned int globalSeed) {GLOBAL_SEED = globalSeed;}

