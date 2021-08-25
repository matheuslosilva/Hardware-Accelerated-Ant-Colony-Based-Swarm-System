#include <antColony.h>

glm::mat4* AntColony::createAntsModelMatrices(vector<Ant*> antsColony)
{
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[POP_SIZE];
    for (int i = 0; i < POP_SIZE; i += 1)
    {

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, antsColony[i]->_theta-glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(antsColony[i]->_size, antsColony[i]->_size, antsColony[i]->_size));
        model[3][0] = (float)antsColony[i]->_x;
        model[3][1] = (float)antsColony[i]->_y;

        modelMatrices[i] = model;
    }

    return modelMatrices;
}

AntColony::AntColony()
{
	for(int i = 0; i < POP_SIZE; i++)
    {						// x  y                      theta                resize velocity  pheromoneIntensity
        ants.push_back(new Ant(0, 0, (glm::radians(360.0f)/(float)POP_SIZE)*i, 0.05f, 0.0001f, 10.0f));
    }

    antsModelMatrices = createAntsModelMatrices(ants);
}

void AntColony::moveAnts(int l)
{
	for(int i = 0; i < POP_SIZE; i++)
    {        
        float oldtheta = ants[i]->_theta;
        ants[i]->move(l);
        antsModelMatrices[i] = glm::rotate(antsModelMatrices[i], ants[i]->_theta - oldtheta, glm::vec3(0.0f, 0.0f, 1.0f));
        antsModelMatrices[i][3][0] = (float)ants[i]->_x;
        antsModelMatrices[i][3][1] = (float)ants[i]->_y;
    }
}