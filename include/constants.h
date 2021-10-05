#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <EBO.h>
#include <VAO.h>
#include <VBO.h>
#include <shader.h>

#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>

#include <ant.h>
#include <antColony.h>

#include <openglContext.h>

#include <camera.h>
#include <sinCosLookup.h>

using namespace std;

// global settings constants
extern unsigned int WIND_WIDTH;
extern unsigned int WIND_HEIGHT;
extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;
extern unsigned int POP_SIZE;
extern int    CHANNEL_COUNT;
extern GLenum PIXEL_FORMAT;
extern int    DATA_SIZE; 
extern GLuint pboIds[2]; // IDs of PBO
extern GLuint textureId; // ID of texture
extern int indexPBO;
extern int nextIndexPBO;
extern unsigned int GLOBAL_SEED;
extern Camera* camera;

#endif
