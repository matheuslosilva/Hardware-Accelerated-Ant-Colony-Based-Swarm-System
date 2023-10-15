#include <openglBuffersManager.h>

void OpenglBuffersManager::createFoodComponents()
{
    ColorVertex foodVertices[4];
    foodVertices[0] = {glm::vec3(-1.0, 1.0, 1.0), glm::vec4(0.0, 0.5, 0.0, 1.0)};
    foodVertices[1] = {glm::vec3(1.0, 1.0, 1.0), glm::vec4(0.0, 0.5, 0.0, 1.0)};
    foodVertices[2] = {glm::vec3(-1.0, -1.0, 1.0), glm::vec4(0.0, 0.5, 0.0, 1.0)};
    foodVertices[3] = {glm::vec3(1.0, -1.0, 1.0), glm::vec4(0.0, 0.5, 0.0, 1.0)};
    VBO foodVerticesVBO(sizeof(foodVertices), (ColorVertex*)foodVertices, GL_STATIC_DRAW);

    foodsTransformationMatricesVBO = new VBO((sizeof(glm::mat4)),(glm::mat4*)&foodsTransformationMatrices[0], GL_STATIC_DRAW);

    foodsVAO = new VAO();
    foodsVAO->bind();
    foodsVAO->linkVBO(foodVerticesVBO, 0, 3, GL_FLOAT, sizeof(ColorVertex), (void*)offsetof(ColorVertex, position));
    foodsVAO->linkVBO(foodVerticesVBO, 1, 4, GL_FLOAT, sizeof(ColorVertex), (void*)offsetof(ColorVertex, color));
    foodsVAO->linkVBOMatrix4(*foodsTransformationMatricesVBO, 2, 4, GL_FLOAT, sizeof(glm::mat4));
    foodsVAO->unbind();    
}

void OpenglBuffersManager::drawFoods(int numberOfFoods, Camera* camera)
{
    //foodsTransformationMatricesVBO->resizeBuffer(sizeof(glm::mat4) * numberOfFoods, (glm::mat4*)&foodsTransformationMatrices[0], GL_STATIC_DRAW);

    shaderAnts->bind();
    shaderAnts->setMat4("view", camera->GetViewMatrix());
    shaderAnts->setMat4("projection", camera->GetProjectionMatrix());

    foodsVAO->bind();
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numberOfFoods);
    foodsVAO->unbind();
}