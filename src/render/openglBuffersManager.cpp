#include <openglBuffersManager.h>
#include <constants.h>
#include <camera.h>

OpenglBuffersManager::OpenglBuffersManager()
{
    shaderAnts = new Shader("shaders/vertexShader.s", "shaders/fragmentShader.s");

    shaderPheromone = new Shader("shaders/texVertexShader.s", "shaders/textureShader.s");
    
    createPheromoneComponents();

}

void OpenglBuffersManager::resetBufferManager()
{
    VAOsFood.clear();
    VAOsNest.clear();
    antsModelMatrices.clear();
    antsMatricesBuffer.clear();
    VAOsAnts.clear();
}

//--------------Environment----------------
void OpenglBuffersManager::createFoodComponents(FoodSource* foodSource)
{
    float Vertices[] = {
        // positions    
        -0.005f,  0.005f, 
         0.005f, -0.005f, 
        -0.005f, -0.005f, 

        -0.005f,  0.005f, 
         0.005f, -0.005f, 
         0.005f,  0.005f, 

    };
    VBO VBOPositions(Vertices, sizeof(Vertices), GL_STATIC_DRAW);

    float Colors[] = {
         0.0f, 0.5f, 0.0f,
         0.0f, 0.5f, 0.0f,
         0.0f, 0.5f, 0.0f,

         0.0f, 0.5f, 0.0f,
         0.0f, 0.5f, 0.0f,
         0.0f, 0.5f, 0.0f 
    };
    VBO VBOColors(Colors, sizeof(Colors), GL_STATIC_DRAW);

    glm::mat4 model = glm::mat4(1.0f);  
    model = glm::scale(model, glm::vec3(foodSource->size, foodSource->size, 1.0));
    model[3][0] = (float)foodSource->posX;
    model[3][1] = (float)foodSource->posY;
    
    VBO* VBOTranformation = new VBO(&model, (sizeof(glm::mat4)), GL_DYNAMIC_DRAW);

    VAO* VAOFood = new VAO();
    // Generates Vertex Array Object and binds it
    VAOFood->Bind();
    VAOFood->LinkVBO(VBOPositions, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
    VAOFood->LinkVBO(VBOColors, 1, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    VAOFood->LinkVBOMatrix4(*VBOTranformation, 2, 4, GL_FLOAT, sizeof(glm::mat4));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    VAOFood->Unbind();    

    VAOsFood.push_back(VAOFood);
}

void OpenglBuffersManager::drawFoods(int nFoods)
{
    shaderAnts->use();
    shaderAnts->setMat4("view", camera->GetViewMatrix());
    shaderAnts->setMat4("projection", camera->GetProjectionMatrix());

    for(int i = 0; i < nFoods; i++)
    {
        VAOsFood[i]->Bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);
        VAOsFood[i]->Unbind();
    }
   
}

void OpenglBuffersManager::createNestComponents(AntColony* antColony)
{
    float Vertices[] = {
        // positions    
        -0.005f,  0.005f, 
         0.005f, -0.005f, 
        -0.005f, -0.005f, 

        -0.005f,  0.005f, 
         0.005f, -0.005f, 
         0.005f,  0.005f, 

    };
    VBO VBOPositions(Vertices, sizeof(Vertices), GL_STATIC_DRAW);

    float Colors[] = {
         0.6f, 0.25f, 0.0f,
         0.6f, 0.25f, 0.0f,
         0.6f, 0.25f, 0.0f,

         0.6f, 0.25f, 0.0f,
         0.6f, 0.25f, 0.0f,
         0.6f, 0.25f, 0.0f 
    };
    VBO VBOColors(Colors, sizeof(Colors), GL_STATIC_DRAW);

    glm::mat4 model = glm::mat4(1.0f);  
    model = glm::scale(model, glm::vec3(antColony->size, antColony->size, 1.0));
    model[3][0] = (float)antColony->posX;
    model[3][1] = (float)antColony->posY;
    
    VBO* VBOTranformation = new VBO(&model, (sizeof(glm::mat4)), GL_DYNAMIC_DRAW);

    VAO* VAONest = new VAO();

    // Generates Vertex Array Object and binds it
    VAONest->Bind();
    VAONest->LinkVBO(VBOPositions, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
    VAONest->LinkVBO(VBOColors, 1, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    VAONest->LinkVBOMatrix4(*VBOTranformation, 2, 4, GL_FLOAT, sizeof(glm::mat4));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    VAONest->Unbind(); 

    VAOsNest.push_back(VAONest);  
}

void OpenglBuffersManager::drawNests(int nNests)
{
    shaderAnts->use();
    shaderAnts->setMat4("view", camera->GetViewMatrix());
    shaderAnts->setMat4("projection", camera->GetProjectionMatrix());

    for(int i = 0; i < nNests; i++)
    {
        VAOsNest[i]->Bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);
        VAOsNest[i]->Unbind();
    }
}

//--------------ANTS------------------------
void OpenglBuffersManager::createAntsModelMatrices(AntColony* antColony)
{
    glm::mat4* modelMatrices = new glm::mat4[antColony->numberOfAnts];

    for (int i = 0; i < antColony->numberOfAnts; i += 1)
    {
        Ant* currentAnt = antColony->ants[i];
        glm::mat4 model = glm::mat4(1.0f);
        
        model = glm::rotate(model, (currentAnt->_theta - glm::radians(90.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(currentAnt->_size, currentAnt->_size, currentAnt->_size));
        model[3][0] = (float)currentAnt->_x;
        model[3][1] = (float)currentAnt->_y;

        modelMatrices[i] = model;
    }

    antsModelMatrices.push_back(modelMatrices);
}


void OpenglBuffersManager::createAntsComponents(AntColony* antColony)
{
    createAntsModelMatrices(antColony);

    float Vertices[] = {
        // positions
         0.0f,  0.025f, 
        -0.05f, -0.05f,
         0.05f, -0.05f
    };
    VBO VBOPositions(Vertices, sizeof(Vertices), GL_STATIC_DRAW);

    float Colors[] = {
        // positions
         0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f
    };
    VBO VBOColors(Colors, sizeof(Colors), GL_STATIC_DRAW);


    VBO* VBOTranformations = new VBO(&antsModelMatrices[0][0], (sizeof(glm::mat4) * antColony->numberOfAnts), GL_DYNAMIC_DRAW);

    VAO *VAOAnts = new VAO();

    // Generates Vertex Array Object and binds it
    VAOAnts->Bind();
    VAOAnts->LinkVBO(VBOPositions, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
    VAOAnts->LinkVBO(VBOColors, 1, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    VAOAnts->LinkVBOMatrix4(*VBOTranformations, 2, 4, GL_FLOAT, sizeof(glm::mat4));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    VAOAnts->Unbind();

    VAOsAnts.push_back(VAOAnts);
    antsMatricesBuffer.push_back(VBOTranformations);

    
}

void OpenglBuffersManager::updateModelAnts(AntColony* antColony)
{

    for(int i = 0; i < antColony->numberOfAnts; i++)
    {       
        Ant* currentAnt = antColony->ants[i];

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::rotate(model, (currentAnt->_theta - glm::radians(90.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(currentAnt->_size, currentAnt->_size, currentAnt->_size));
        model[3][0] = (float)currentAnt->_x;
        model[3][1] = (float)currentAnt->_y;

        antsModelMatrices[antColony->id][i] = model;
    }
}

void OpenglBuffersManager::drawAnts(AntColony* antColony)
{
    shaderAnts->use();
    shaderAnts->setMat4("view", camera->GetViewMatrix());
    shaderAnts->setMat4("projection", camera->GetProjectionMatrix());

    //-------------------------------------------------------------------------------
    antsMatricesBuffer[antColony->id]->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4)*antColony->numberOfAnts, antsModelMatrices[antColony->id]); // FIRST BOTTLE NECK
    antsMatricesBuffer[antColony->id]->Unbind();

    VAOsAnts[antColony->id]->Bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, antColony->numberOfAnts); // 100000 triangles of 3 vertices each
    VAOsAnts[antColony->id]->Unbind();
    //----------------------------------------------------------------
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
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

void OpenglBuffersManager::createPheromoneComponents()
{
    VAO *newVAOPheromone = new VAO();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float VerticesTexture[] = {
        // positions               // texture coords
        -1.0f,  1.0f, 0.0f,     0.0f, 1.0f,  // top left
        1.0f,  1.0f, 0.0f,      1.0f, 1.0f, // top right
        1.0f,  -1.0f, 0.0f,     1.0f, 0.0f, // bottom right
        -1.0f,  -1.0f, 0.0f,     0.0f, 0.0f // bottom left
    };
    VBO VBOVerticesTexture(VerticesTexture, sizeof(VerticesTexture), GL_STATIC_DRAW);

    unsigned int indicesTexture[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    EBO EBOIndicesTexture(indicesTexture, sizeof(indicesTexture));

    newVAOPheromone->Bind();
    EBOIndicesTexture.Bind();
    newVAOPheromone->LinkVBO(VBOVerticesTexture, 0, 3, GL_FLOAT, 5* sizeof(float), (void*)0);
    newVAOPheromone->LinkVBO(VBOVerticesTexture, 2, 2, GL_FLOAT, 5* sizeof(float), (void*)(3 * sizeof(float)));
    newVAOPheromone->Unbind();

    createTextureBuffer();
    createPixelBuffers();

    VAOPheromone = newVAOPheromone;
}

void OpenglBuffersManager::swapPixelBuffers(vector<uint8_t> pheromoneMatrix)
{
    // In dual PBO mode, increment current index first then get the next index
    indexPBO = (indexPBO + 1) % 2;
    nextIndexPBO = (indexPBO + 1) % 2;

    // bind PBO to update pixel values
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[nextIndexPBO]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
    pixelMap = (GLbitfield*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY); 

    memcpy(pixelMap, pheromoneMatrix.data(), DATA_SIZE); // smaller BOTTLE NECK
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // release pointer to mapping buffer

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, textureId);

    // copy pixels from PBO to texture object
    // Use offset instead of pointer.
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT, PIXEL_FORMAT, GL_UNSIGNED_BYTE, 0); // Big BOTTLE NECK if the screen grows

    // It is good idea to release PBOs with ID 0 after use.
    // Once bound with 0, all pixel operations behave normal ways.
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void OpenglBuffersManager::drawPheromone(vector<uint8_t> pheromoneMatrix)
{
    swapPixelBuffers(pheromoneMatrix);

    shaderPheromone->use();
    shaderPheromone->setMat4("view", camera->GetViewMatrix());
    shaderPheromone->setMat4("projection", camera->GetProjectionMatrix());

    VAOPheromone->Bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    VAOPheromone->Unbind();
   
}
