#include <openglBuffersManager.h>

int    CHANNEL_COUNT    = 4;
GLenum PIXEL_FORMAT     = GL_RGBA;
int    DATA_SIZE        = PIXEL_WIDTH * PIXEL_HEIGHT * CHANNEL_COUNT; // Size of the pixel data content
GLuint pboIds[2]; // IDs of PBO
GLuint textureId; // ID of texture
int indexPBO = 0;
int nextIndexPBO = 0;

OpenglBuffersManager::OpenglBuffersManager()
{
    shaderAnts = new Shader();
    shaderAnts->createProgramLinkShaders("src/opengl/shaders/antVShader.s", "src/opengl/shaders/antFShader.s", NULL, NULL);

    shaderPheromone = new Shader();
    shaderPheromone->createProgramLinkShaders("src/opengl/shaders/pheromoneVShader.s", "src/opengl/shaders/pheromoneFShader.s", NULL, NULL);
    
    createAnthillComponents();
    createFoodComponents();
    createAntComponents();
    createPheromoneComponents();
}

void OpenglBuffersManager::resetBufferManager()
{
    foodsTransformationMatrices.clear();
    anthillsTransformationMatrices.clear();
    antsTransformationMatrices.clear();
}


void OpenglBuffersManager::addElement(vector<glm::mat4>* transformationMatrices, float size, float posX, float posY, float theta)
{
    glm::mat4 model = glm::mat4(1.0f);
        
    model = glm::scale(model, glm::vec3(size, size, 1.0));
    model = glm::rotate(model, theta, glm::vec3(0.0f, 0.0f, 1.0f));

    model[3][0] = (float)posX;
    model[3][1] = (float)posY;

    transformationMatrices->push_back(model);
}

void OpenglBuffersManager::updateBuffer(VBO* vertexBufferObject, int numberOfElements, vector <glm::mat4> transformationMatrices, GLenum usage)
{
    vertexBufferObject->resizeBuffer(sizeof(glm::mat4) * numberOfElements, (glm::mat4*)&transformationMatrices[0], usage);
}

void OpenglBuffersManager::updateBufferData(VBO* vertexBufferObject, int numberOfElements, vector <glm::mat4> transformationMatrices)
{
    vertexBufferObject->subBufferData(sizeof(glm::mat4) * numberOfElements, (glm::mat4*)&transformationMatrices[0]); // FIRST BOTTLE NECK
}

















// TODO pixel texture buffer

void OpenglBuffersManager::createPheromoneComponents()
{
    TextureVertex pheromoneVertices[4];
    pheromoneVertices[0] = {glm::vec3(-1.0, 1.0, 1.0), glm::vec2(0.0, 1.0)};
    pheromoneVertices[1] = {glm::vec3(1.0, 1.0, 1.0), glm::vec2(1.0, 1.0)};
    pheromoneVertices[2] = {glm::vec3(1.0, -1.0, 1.0), glm::vec2(1.0, 0.0)};
    pheromoneVertices[3] = {glm::vec3(-1.0, -1.0, 1.0), glm::vec2(0.0, 0.0)};
    VBO pheromoneVerticesVBO(sizeof(pheromoneVertices), (TextureVertex*)pheromoneVertices, GL_STATIC_DRAW);

    unsigned int indicesVertices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    EBO indicesVerticesEBO(sizeof(indicesVertices), indicesVertices);

    pheromoneVAO = new VAO();
    pheromoneVAO->bind();
    indicesVerticesEBO.bind();
    pheromoneVAO->linkVBO(pheromoneVerticesVBO, 0, 3, GL_FLOAT, sizeof(TextureVertex), (void*)offsetof(TextureVertex, vertexPosition));
    pheromoneVAO->linkVBO(pheromoneVerticesVBO, 1, 2, GL_FLOAT, sizeof(TextureVertex), (void*)offsetof(TextureVertex, texturePosition));
    pheromoneVAO->unbind();
    indicesVerticesEBO.unbind();

    createTextureBuffer();
    createPixelBuffers();
}

//------------PHEROMONE----------------------
void OpenglBuffersManager::createTextureBuffer()
{
    GLubyte* imageData = 0;
    // allocate texture buffer
    imageData = new GLubyte[DATA_SIZE];
    memset(imageData, 0, DATA_SIZE);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, PIXEL_WIDTH, PIXEL_HEIGHT, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenglBuffersManager::createPixelBuffers()
{
    // Create two pixel buffers
    glGenBuffers(2, pboIds);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[0]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[1]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void OpenglBuffersManager::swapPixelBuffers(uint8_t* pheromoneMatrix)
{
    // In dual PBO mode, increment current index first then get the next index
    indexPBO = (indexPBO + 1) % 2;
    nextIndexPBO = (indexPBO + 1) % 2;

    // bind PBO to update pixel values
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[nextIndexPBO]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
    pixelMap = (GLbitfield*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY); 

    memcpy(pixelMap, pheromoneMatrix, DATA_SIZE); // smaller BOTTLE NECK
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // release pointer to mapping buffer

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, textureId);

    // copy pixels from PBO to texture object
    // Use offset instead of pointer.
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, PIXEL_WIDTH, PIXEL_HEIGHT, PIXEL_FORMAT, GL_UNSIGNED_BYTE, 0); // Big BOTTLE NECK if the screen grows

    // It is good idea to release PBOs with ID 0 after use.
    // Once bound with 0, all pixel operations behave normal ways.
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void OpenglBuffersManager::drawPheromone(uint8_t* pheromoneMatrix, Camera* camera)
{
    swapPixelBuffers(pheromoneMatrix);

    shaderPheromone->bind();
    shaderPheromone->setMat4("view", camera->GetViewMatrix());
    shaderPheromone->setMat4("projection", camera->GetProjectionMatrix());

    pheromoneVAO->bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    pheromoneVAO->unbind();
   
}
