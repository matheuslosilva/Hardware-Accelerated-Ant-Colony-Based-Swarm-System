#include <openglBuffersManager.h>

void OpenglBuffersManager::createAntComponents()
{
    ColorVertex antVertices[3];
    antVertices[0] = {glm::vec3(-0.8, -1.0, 1.0), glm::vec4(0.5, 0.5, 0.5, 1.0)};
    antVertices[1] = {glm::vec3(0.8, -1.0, 1.0), glm::vec4(0.5, 0.5, 0.5, 1.0)};
    antVertices[2] = {glm::vec3(0.0, 1.0, 1.0), glm::vec4(0.5, 0.5, 0.5, 1.0)};
    VBO antVerticesVBO(sizeof(antVertices), (ColorVertex*)antVertices, GL_STATIC_DRAW);

    antsTransformationMatricesVBO = new VBO(sizeof(glm::mat4), (glm::mat4*)&antsTransformationMatrices[0], GL_DYNAMIC_DRAW);

    antsVAO = new VAO();
    antsVAO->bind();
    antsVAO->linkVBO(antVerticesVBO, 0, 3, GL_FLOAT, sizeof(ColorVertex), (void*)offsetof(ColorVertex, position));
    antsVAO->linkVBO(antVerticesVBO, 1, 4, GL_FLOAT, sizeof(ColorVertex), (void*)offsetof(ColorVertex, color));
    antsVAO->linkVBOMatrix4(*antsTransformationMatricesVBO, 2, 4, GL_FLOAT, sizeof(glm::mat4));
    antsVAO->unbind();  
}

void OpenglBuffersManager::drawAnts(int numberOfAnts, Camera* camera)
{

    shaderAnts->bind();
    shaderAnts->setMat4("view", camera->GetViewMatrix());
    shaderAnts->setMat4("projection", camera->GetProjectionMatrix());

    antsTransformationMatricesVBO->subBufferData(sizeof(glm::mat4) * numberOfAnts, (glm::mat4*)&antsTransformationMatrices[0]); // FIRST BOTTLE NECK

    antsVAO->bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, numberOfAnts); // 100000 triangles of 3 vertices each
    antsVAO->unbind();
}

void OpenglBuffersManager::updateModelAnts(int numberOfAnts, vector<Ant*> ants)
{
    glm::mat4 model;
    Ant* currentAnt;

    for (int i = 0; i < numberOfAnts; i++)
    {       
        currentAnt = ants[i];

        model = glm::mat4(1.0f);

        model = glm::scale(model, glm::vec3(currentAnt->size, currentAnt->size, 1.0));
        model = glm::rotate(model, (currentAnt->theta-glm::radians(90.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
        model[3][0] = (float)currentAnt->posX;
        model[3][1] = (float)currentAnt->posY;

        antsTransformationMatrices[i] = model;
    }
}
