#ifndef OPENGLBUFFERSMANAGER_H
#define OPENGLBUFFERSMANAGER_H

#include <shader.h>
#include <VAO.h>

#include <UI.h>
#include <camera.h>

#include <ant.h>

// Pixel mapping (for pheromone)
extern int    CHANNEL_COUNT;
extern GLenum PIXEL_FORMAT;
extern int    DATA_SIZE; // Size of the pixel data content
extern GLuint pboIds[2]; // IDs of PBO
extern GLuint textureId; // ID of texture
extern int indexPBO;
extern int nextIndexPBO;

using namespace std;

typedef struct
{
	glm::vec3 position;
	glm::vec4 color;
}ColorVertex;

typedef struct
{
	glm::vec3 vertexPosition;
	glm::vec2 texturePosition;
}TextureVertex; 

class OpenglBuffersManager
{
	public:

	  	Shader* shaderAnts;
	    Shader* shaderPheromone;

	    vector <glm::mat4> foodsTransformationMatrices;
	    VBO* foodsTransformationMatricesVBO;
	    VAO* foodsVAO;

	    vector <glm::mat4> anthillsTransformationMatrices;
	    VBO* anthillsTransformationMatricesVBO;
	    VAO* anthillsVAO;

	    vector <glm::mat4> antsTransformationMatrices;
	    VBO* antsTransformationMatricesVBO;
	    VAO* antsVAO;

	    VAO* pheromoneVAO;    
	    GLbitfield* pixelMap;  

		OpenglBuffersManager();
		void resetBufferManager();

		void addElement(vector <glm::mat4>* transformationMatrices, float size, float theta, float posX, float posY);
		void updateBuffer(VBO* vertexBufferObject, int numberOfElements, vector <glm::mat4> transformationMatrices, GLenum usage);
		void updateBufferData(VBO* vertexBufferObject, int numberOfElements, vector <glm::mat4> transformationMatrices);

		void createFoodComponents();
		void drawFoods(int numberOfFoods, Camera* camera);
	
		void createAnthillComponents();
		void drawAnthills(int numberOfAnthills, Camera* camera);

		void createAntComponents();
		void drawAnts(int numberOfAnts, Camera* camera);
		


		void updateModelAnts(int numberOfAnts, vector<Ant*> ants);

		void createPheromoneComponents();
		void createTextureBuffer();
		void createPixelBuffers();
		void swapPixelBuffers(uint8_t* pheromoneMatrix);
		void drawPheromone(uint8_t* pheromoneMatrix, Camera* camera);

};

#endif