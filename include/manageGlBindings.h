#ifndef MANAGEGLBINDINGS_H
#define MANAGEGLBINDINGS_H

#include <constants.h>

using namespace std;

VBO createAntsVertexObjects(VAO VAOAnts, glm::mat4* antsModelMatrices);
VAO createPheromoneVertexObjects();
void createPixelBuffers();

#endif