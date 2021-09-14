#include <constants.h>

// settings constants

// Environment
unsigned int WIND_WIDTH  = 1000;
unsigned int WIND_HEIGHT = 1000;
unsigned int SCR_WIDTH  = 3000;
unsigned int SCR_HEIGHT = 3000;
unsigned int POP_SIZE   = 100000;
unsigned int GLOBAL_SEED = 6454848;
Camera camera;

// Pixel mapping (for pheromone)
int    CHANNEL_COUNT    = 4;
GLenum PIXEL_FORMAT     = GL_RGBA;
int    DATA_SIZE        = SCR_WIDTH * SCR_HEIGHT * CHANNEL_COUNT; // Size of the pixel data content
GLuint pboIds[2]; // IDs of PBO
GLuint textureId; // ID of texture
int indexPBO = 0;
int nextIndexPBO = 0;
