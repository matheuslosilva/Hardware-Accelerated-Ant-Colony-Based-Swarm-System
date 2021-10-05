#include <constants.h>
#include <openglBuffersManager.h>

using namespace std::chrono;
using namespace std;


void updatePixels(vector<uint8_t> &pheromoneMatrix, float x, float y, int pheromoneType)
{
    int xn, yn, index;

    xn = ((SCR_WIDTH/2) + x * (SCR_WIDTH/2));
    yn = ((SCR_HEIGHT/2) + y * (SCR_HEIGHT/2));
    index = (yn * SCR_WIDTH) + xn;

    if(pheromoneType == 1)
    {
        if(pheromoneMatrix[index*4] < 230) pheromoneMatrix[index*4] += 20;
    }
    else if(pheromoneType == 2)
    {
         if(pheromoneMatrix[(index*4)+2] < 230) pheromoneMatrix[(index*4)+2] += 20;
    }       

}

void pheromoneEvaporation(vector<uint8_t> &pheromoneMatrix)
{

    for(int i = 0; i < DATA_SIZE; i+=4)
    {
        if(pheromoneMatrix[i]>0) pheromoneMatrix[i]--;
    }
    for(int i = 1; i < DATA_SIZE; i+=4)
    {
        if(pheromoneMatrix[i]>0) pheromoneMatrix[i]--;
    }
    for(int i = 2; i < DATA_SIZE; i+=4)
    {
        if(pheromoneMatrix[i]>0) pheromoneMatrix[i]--;
    }
}

int main()
{
    srand(GLOBAL_SEED);

    AntColony *antColony = new AntColony();

    vector<uint8_t> pheromoneMatrix(DATA_SIZE);

    OpenglContext* openglContext = new OpenglContext();

    openglContext->init();

    OpenglBuffersManager* openglBuffersManager = new OpenglBuffersManager(antColony);

    camera->defMonitor();

    // build and compile shaders
    // -------------------------
   
    int openGlRenderUpdateFrameRate = 2000;
    int pheromoneEvaporationFrameRate = 5;
    int pheromoneMatrixUpdatePixelsFrameRate = 5;

    int frames = 0;
    // render loop
    // -------------------------------------------------------------------
    while (!glfwWindowShouldClose(openglContext->antColonyWindow))
    {

        

       
        

        // Environment
        //----------------------------------------------------------------
        // input
        openglContext->processInput(camera);

        antColony->moveAnts(openglContext->frameCounter, pheromoneMatrix); // TODO CUDA
       
         

        if(openglContext->frameCounter % pheromoneEvaporationFrameRate == 0)
        { 
            pheromoneEvaporation(pheromoneMatrix); 
        }
       

        if(openglContext->frameCounter % pheromoneMatrixUpdatePixelsFrameRate == 0)
        { 
          
            // update data directly on the mapped buffer
            for(int i = 0; i < POP_SIZE; i++)
            {
               updatePixels(pheromoneMatrix, antColony->ants[i]->_x, antColony->ants[i]->_y, antColony->ants[i]->_pheromoneType); 
               // testeSensor(pheromoneMatrix, antColony->ants[i]->_xSensorR, antColony->ants[i]->_ySensorR);
                //testeSensor(pheromoneMatrix, antColony->ants[i]->_xSensorL, antColony->ants[i]->_ySensorL);
            }
            
        }
         
        if(openglContext->frameCounter % openGlRenderUpdateFrameRate == 0) 
        {
            if(frames == 0 )openglContext->debugExecutionTimeStart();
            cout << "FRAMES: " << frames << endl;
            openglContext->pre_render();

            antColony->updateModelAnts();           

            openglBuffersManager->drawAnts(antColony, camera);
    
            openglBuffersManager->drawPheromone(pheromoneMatrix, camera);

            if(frames != 0 )openglContext->debugExecutionTimeStop("pheromone");

 
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        openglContext->post_render();

        frames++;

         openglContext->frameCounter++;
        openglContext->frameCounter %= 1000;

    }

    openglContext->end();
    return 0;
}

