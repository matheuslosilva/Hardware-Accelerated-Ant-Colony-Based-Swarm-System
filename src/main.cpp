#include <openglContext.h>
#include <iostream>

int main()
{
    OpenglContext* openglContext = new OpenglContext();
    openglContext->init();

    OpenglBuffersManager* openglBuffersManager =  new OpenglBuffersManager();

    srand(GLOBAL_SEED);

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

