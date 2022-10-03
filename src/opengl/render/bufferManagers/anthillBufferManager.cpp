#include <openglBuffersManager.h>
#include <iostream>
void OpenglBuffersManager::createAnthillComponents()
{
    ColorVertex anthillVertices[4];
    anthillVertices[0] = {glm::vec3(-1.0, 1.0, 1.0), glm::vec4(0.5, 0.0, 0.0, 1.0)};
    anthillVertices[1] = {glm::vec3(1.0, 1.0, 1.0), glm::vec4(0.5, 0.0, 0.0, 1.0)};
    anthillVertices[2] = {glm::vec3(-1.0, -1.0, 1.0), glm::vec4(0.5, 0.0, 0.0, 1.0)};
    anthillVertices[3] = {glm::vec3(1.0, -1.0, 1.0), glm::vec4(0.5, 0.0, 0.0, 1.0)};
    VBO anthillVerticesVBO(sizeof(anthillVertices), (ColorVertex*)anthillVertices, GL_STATIC_DRAW);

    anthillsTransformationMatricesVBO = new VBO(sizeof(glm::mat4), (glm::mat4*)&anthillsTransformationMatrices[0], GL_STATIC_DRAW);

    anthillsVAO = new VAO();
    anthillsVAO->bind();
    anthillsVAO->linkVBO(anthillVerticesVBO, 0, 3, GL_FLOAT, sizeof(ColorVertex), (void*)offsetof(ColorVertex, position));
    anthillsVAO->linkVBO(anthillVerticesVBO, 1, 4, GL_FLOAT, sizeof(ColorVertex), (void*)offsetof(ColorVertex, color));
    anthillsVAO->linkVBOMatrix4(*anthillsTransformationMatricesVBO, 2, 4, GL_FLOAT, sizeof(glm::mat4));
    anthillsVAO->unbind();     
}

void OpenglBuffersManager::drawAnthills(int numberOfAnthills, Camera* camera)
{
    shaderAnts->bind();
    shaderAnts->setMat4("view", camera->GetViewMatrix());
    shaderAnts->setMat4("projection", camera->GetProjectionMatrix());

    //anthillsTransformationMatricesVBO->subBufferData(sizeof(glm::mat4) * numberOfAnthills, (glm::mat4*)&anthillsTransformationMatrices[0]); // FIRST BOTTLE NECK

    anthillsVAO->bind();
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numberOfAnthills);
    anthillsVAO->unbind();
}