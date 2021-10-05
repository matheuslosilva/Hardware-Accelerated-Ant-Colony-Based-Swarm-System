#ifndef OPENGLBUFFERSMANAGER_H
#define OPENGLBUFFERSMANAGER_H

#include <constants.h>


using namespace std;

class OpenglBuffersManager
{
	
	public:
	  	Shader* shaderAnts;
	    Shader* shaderPheromone;

	    VAO* VAOAnts = new VAO();
	    VBO* AntsMatricesBuffer;

	    VAO* VAOPheromone;    
	    GLbitfield* pixelMap;  

	  public:
		OpenglBuffersManager(AntColony *antColony);

		VBO* createAntsComponents(glm::mat4* antsModelMatrices);
		void drawAnts(AntColony* antColony, Camera* camera);

		void createTextureBuffer();
		void createPixelBuffers();
		VAO* createPheromoneComponents();
		void swapPixelBuffers(vector<uint8_t> pheromoneMatrix);
		void drawPheromone(vector<uint8_t> pheromoneMatrix, Camera* camera);
};

#endif