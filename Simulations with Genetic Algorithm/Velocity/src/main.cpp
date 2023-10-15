#include <openglContext.h>
#include <iostream>

int main()
{
    OpenglContext* openglContext = new OpenglContext();
    openglContext->init();

    OpenglBuffersManager* openglBuffersManager =  new OpenglBuffersManager();

    srand(GLOBAL_SEED);

    //----------------------------------------------------------------------------------
    // Starting the simulation
    openglContext->userInterface->stateSimulation = RUNNING;

    openglContext->parameterAssigner = new ParameterAssigner("src/swarmEnvironment/experiments/simulation.json");
    openglContext->environment = new Environment(openglContext->parameterAssigner);
    openglContext->environment->initializeEnvironment(openglBuffersManager);

    openglContext->environment->createNest(0, openglBuffersManager);
    openglContext->environment->createFoodSource(0, openglBuffersManager);
    openglContext->environment->createFoodSource(1, openglBuffersManager);
    openglContext->environment->createFoodSource(2, openglBuffersManager);
    openglContext->environment->createFoodSource(3, openglBuffersManager);
    openglContext->environment->createAnt(0, openglBuffersManager);
    //----------------------------------------------------------------------------------

    openglContext->run(openglBuffersManager);

    openglContext->end();
    return 0;
}




/*void debugExecutionTimeStart()
{
    startDebugTimer = std::chrono::high_resolution_clock::now(); // Execution time counte
}

void debugExecutionTimeStop(std::string debugText)
{
    // Debug the execution time to improve optimization
    stopDebugTimer = high_resolution_clock::now(); 
    durationTimer = duration_cast<microseconds>(stopDebugTimer - startDebugTimer); 
    cout << "Execution time of "<< debugText << ": " << durationTimer.count()/1000.0f << endl; 
}*/

