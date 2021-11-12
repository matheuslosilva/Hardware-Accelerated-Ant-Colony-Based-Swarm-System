#ifndef OPENGLBUFFERSMANAGER_H
#define OPENGLBUFFERSMANAGER_H

#include <EBO.h>
#include <VAO.h>
#include <VBO.h>
#include <shader.h>
#include <camera.h>
#include <ant.h>
#include <foodsource.h>


using namespace std;

class OpenglBuffersManager
{
	
	public:

	  	Shader* shaderAnts;
	    Shader* shaderPheromone;

	    vector < VAO* > VAOsFood;

	    vector < VAO* > VAOsNest;

	    vector < glm::mat4* > antsModelMatrices;
	    vector < VBO* > antsMatricesBuffer;
	    vector < VAO* > VAOsAnts;

	    VAO* VAOPheromone;    
	    GLbitfield* pixelMap;  

	  public:

		OpenglBuffersManager();
		void resetBufferManager();
		
		void createFoodComponents(FoodSource* foodSource);
		void createNestComponents(AntColony* antColony);
		void drawFoods(int nFoods);
		void drawNests(int nNests);

		void createAntsModelMatrices(AntColony* antsColony);
		void createAntsComponents(AntColony* antColony);
		void updateModelAnts(AntColony* antColony);

		void createTextureBuffer();
		void createPixelBuffers();
		void createPheromoneComponents();
		void swapPixelBuffers(vector<uint8_t> pheromoneMatrix);

		void drawPheromone(vector<uint8_t> pheromoneMatrix);
		void drawAnts(AntColony* antColony);

};

#endif