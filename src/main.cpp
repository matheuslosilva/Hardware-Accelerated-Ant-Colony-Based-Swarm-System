#include <openglContext.h>
#include <openglBuffersManager.h>
#include <UI.h>
#include <environment.h>
#include <constants.h>
#include <iostream>
#include <unistd.h>

#define SIMULATION_PERIOD 10000

unsigned int microsecond = 1000000;

using namespace std::chrono;
using namespace std;

int main(int argc, char** argv)
{

    float antSpeed = stoi(argv[1])/10000.0;

    srand(GLOBAL_SEED);
    
    OpenglContext* openglContext = new OpenglContext();

    UI* userInterface = new UI();

    openglContext->init();

    userInterface->init(openglContext->antColonyWindow);

    camera->defMonitor();

    OpenglBuffersManager* openglBuffersManager =  new OpenglBuffersManager();

    Environment* environment = new Environment(7, 40);
    environment->initializeEnvironment(openglBuffersManager, antSpeed);

    int maxMovements = SIMULATION_PERIOD;
    int currentMovement = 0;
    bool maxReached = false;

    int openGlRenderUpdateFrameRate = 5;

    // render loop
    // -------------------------------------------------------------------
    while (!glfwWindowShouldClose(openglContext->antColonyWindow) && userInterface->stateSimulation != CLOSED && !maxReached)
    {
        switch(userInterface->stateSimulation)
        {
            case STARTED:

                while(userInterface->stateSimulation == STARTED && !glfwWindowShouldClose(openglContext->antColonyWindow))
                {
                    openglContext->pre_render(); // Framecounter++ here

                    openglContext->processInput(userInterface); 

                    openglBuffersManager->drawPheromone(environment->pheromoneMatrix);

                    userInterface->run();
                    openglContext->post_render();

                    userInterface->stateSimulation = RUNNING;
                }

            break;

            case STOPPED:

                environment->resetEnvironment();
                openglBuffersManager->resetBufferManager();

                while(userInterface->stateSimulation == STOPPED && !glfwWindowShouldClose(openglContext->antColonyWindow))
                {
                    openglContext->pre_render(); // Framecounter++ here

                    openglContext->processInput(userInterface); 

                    openglBuffersManager->drawPheromone(environment->pheromoneMatrix);
                   
                    if (userInterface->stateOfAction == PLACING_NEST)
                    {  
                        environment->createNest(userInterface, openglBuffersManager);

                        userInterface->stateOfAction = DOING_NOTHING;    
                    }

                    if (userInterface->stateOfAction == PLACING_FOOD)
                    {  
                        environment->createFoodSource(userInterface, openglBuffersManager);

                        userInterface->stateOfAction = DOING_NOTHING;    
                    }

                    userInterface->run();
                    openglContext->post_render();
                }

            break;

            case RUNNING:

                while(userInterface->stateSimulation == RUNNING && !glfwWindowShouldClose(openglContext->antColonyWindow))
                {
                    //openglContext->d ebugExecutionTimeStart();
                    currentMovement += 1;
                    openglContext->processInput(userInterface); 

                    environment->moveAnts(openglContext->frameCounter); // TODO CUDA

                    environment->updatePheromonePixels(openglContext->frameCounter); 
                     
                    environment->pheromoneEvaporation(openglContext->frameCounter);  

                   
                    if(userInterface->turnOnGraphics && openglContext->frameCounter % openGlRenderUpdateFrameRate == 0) 
                    {   
                        openglContext->pre_render();// Framecounter++ here

                        environment->draw(openglBuffersManager); 
                    }

                    userInterface->run();

                    openglContext->post_render();

                    if (currentMovement == maxMovements) {
                        maxReached = true;
                        std::cout << environment->Nests[0]->food;
                        break;
                    }
                    
                    //openglContext->debugExecutionTimeStop("total");

                   // usleep(0.01 * microsecond);
                }   

            break;

            case PAUSED:

                camera = new Camera();
                camera->defMonitor();
                while(userInterface->stateSimulation == PAUSED && !glfwWindowShouldClose(openglContext->antColonyWindow))
                {
                    openglContext->processInput(userInterface); 

                    openglContext->pre_render(); // Framecounter++ here

                    environment->draw(openglBuffersManager);

                    userInterface->run();

                    openglContext->post_render();
                }

            break;
        }
    }

    openglContext->end();
    return 0;
}

