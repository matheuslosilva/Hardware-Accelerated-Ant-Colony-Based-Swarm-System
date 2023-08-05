#include <constants.h>
#include <limits.h>

unsigned int PIXEL_WIDTH = 2000;
unsigned int PIXEL_HEIGHT = 2000;
unsigned int GLOBAL_SEED = 0;

float VELOCITY = 0.0004;
unsigned int RED_PHEROMONE_INTENSITY = 60;
unsigned int GREEN_PHEROMONE_INTENSITY = 60;
int TIMEOUT = 400;
int PHEROMONE_EVAPORATION = 15;
int CHANGE_TO_EXPLORER = INT_MAX * 0.00001;
int CHANGE_TO_CARRIER = INT_MAX * 0.00001;
int CHANGE_TO_NESTCARRIER = INT_MAX * 0.00001;
int CHANGE_TO_FOLLOWGREEN = INT_MAX * 0.00001;

// Environment
void setScrWidth(unsigned int scrWidth) {PIXEL_WIDTH  = scrWidth;}
void setScrHeight(unsigned int scrHeight) {PIXEL_HEIGHT = scrHeight;}
void setGlobalSeed(unsigned int globalSeed) {GLOBAL_SEED = globalSeed;}

