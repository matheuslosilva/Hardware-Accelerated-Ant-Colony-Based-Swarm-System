#include <environment.h>

#include <chrono>

using namespace std::chrono;
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//                   copy an image data to texture buffer
///////////////////////////////////////////////////////////////////////////////
void updatePixels(vector<int> &dst, float x, float y)
{
    int* ptr = dst.data();
    float xn, yn;

    xn = x < 0 ? (SCR_WIDTH/2) - (-x*(SCR_WIDTH/2)) : x*(SCR_WIDTH/2)+(SCR_WIDTH/2);
    yn = y < 0 ? (SCR_HEIGHT/2) - (-y*(SCR_HEIGHT/2)) : y*(SCR_HEIGHT/2)+(SCR_HEIGHT/2);

    int actualColor = ptr[(((int)yn)*SCR_WIDTH+(int)xn)];

    bitset<32> r = (bitset<32>)actualColor & (bitset<32>)255;
    bitset<32> g = 0;
    bitset<32> b = (bitset<32>)actualColor >> 16 & (bitset<32>) 255;
    bitset<32> a = 0;

    int ri = (int)r.to_ulong();
    if(ri < 230)ri += 20;
    r = (bitset<32>) ri;

    bitset<32> rgba = ((a<<24)|(b<<16)|(g<<8)|r);

    ptr[(((int)yn)*SCR_WIDTH+(int)xn)] = (GLbitfield)rgba.to_ulong();
}

void updatePixels2(vector<int> &dst, float x, float y)
{
    int* ptr = dst.data();

    int actualColor = ptr[(((int)y)*SCR_WIDTH+(int)x)];

    bitset<32> r = (bitset<32>)actualColor & (bitset<32>)255;
    bitset<32> g = 0;
    bitset<32> b = (bitset<32>)actualColor >> 16 & (bitset<32>) 255;
    bitset<32> a = 0;

    int ri = (int)r.to_ulong();
    if(ri > 1)ri -= 1;
    r = (bitset<32>) ri;

    bitset<32> rgba = ((a<<24)|(b<<16)|(g<<8)|r);

    ptr[(((int)y)*SCR_WIDTH+(int)x)] = (GLbitfield)rgba.to_ulong();
}

int main()
{
    GLFWwindow* window = glfwInitialize();

    // build and compile shaders
    // -------------------------
    Shader shaderAnts("shaders/vertexShader.s", "shaders/fragmentShader.s");

    Shader shaderPheromone("shaders/texVertexShader.s", "shaders/textureShader.s");
    
    AntColony antColony = AntColony();

    VAO VAOAnts;
    
    VBO AntsMatricesBuffer = createAntsVertexObjects(VAOAnts, antColony.antsModelMatrices);

    VAO VAOPheromone = createPheromoneVertexObjects();
    GLbitfield* pixelMap;

    vector<int> pheromoneMatrix(DATA_SIZE/4);

    glm::mat4 projection;
    glm::mat4 view;
   
    int frameCounter= 0;
    float attFrame = glfwGetTime();
    float openGlUpdate = 0.1;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // Environment
        //----------------------------------------------------------------
        // input
        processInput(window);

        frameCounter++; // Frame Counter
        auto start = high_resolution_clock::now(); // Execution time counter

        // Configure transformation matrices
        projection = glm::perspective(glm::radians(camera.Zoom), (float)WIND_WIDTH / (float)WIND_HEIGHT, 0.001f, 100.0f);
        view = camera.GetViewMatrix();
        
        //----------------------------------------------------------------

        // Ants
        //----------------------------------------------------------------
        antColony.moveAnts(frameCounter); // TODO CUDA

        /*for(int xi = 0; xi < SCR_WIDTH; xi++)
        {
            for(int yi = 0; yi < SCR_HEIGHT; yi++)
            {
                updatePixels2(pheromoneMatrix, xi, yi); 
            }
        }*/

        if(frameCounter%1==0)
        { 
            // update data directly on the mapped buffer
            for(int i = 0; i < POP_SIZE; i++)
            {
                updatePixels(pheromoneMatrix, antColony.ants[i]->_x, antColony.ants[i]->_y); 
            }
        }

        if(glfwGetTime() - attFrame > openGlUpdate) 
        {
            antColony.updateModelAnts();
           
            
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shaderAnts.use();
            shaderAnts.setMat4("view", view);
            shaderAnts.setMat4("projection", projection);

            VAOAnts.Bind();

            AntsMatricesBuffer.Bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4)*POP_SIZE, antColony.antsModelMatrices); // FIRST BOTTLE NECK
            AntsMatricesBuffer.Unbind();

            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, POP_SIZE); // 100000 triangles of 6 vertices each
            VAOAnts.Unbind();
            //----------------------------------------------------------------


            // ########################### TODO ##############################
            // Pheromone
            // In dual PBO mode, increment current index first then get the next index
            indexPBO = (indexPBO + 1) % 2;
            nextIndexPBO = (indexPBO + 1) % 2;

            //----------------------------------------------------------------
            // bind PBO to update pixel values
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[nextIndexPBO]);
            glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
            pixelMap = (GLbitfield*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

            memcpy(pixelMap, pheromoneMatrix.data(), DATA_SIZE); // smaller BOTTLE NECK
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // release pointer to mapping buffer
            
            //----------------------------------------------------------------

            //----------------------------------------------------------------
            // bind Texture
            glBindTexture(GL_TEXTURE_2D, textureId);
            // copy pixels from PBO to texture object
            // Use offset instead of pointer.
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT, PIXEL_FORMAT, GL_UNSIGNED_BYTE, 0); // Big BOTTLE NECK if the screen grows
            
            // It is good idea to release PBOs with ID 0 after use.
            // Once bound with 0, all pixel operations behave normal ways.
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            //----------------------------------------------------------------
            
            // render container
            shaderPheromone.use();
            shaderPheromone.setMat4("view", view);
            shaderPheromone.setMat4("projection", projection);

            VAOPheromone.Bind();

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            VAOPheromone.Unbind();
            //--------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            attFrame = glfwGetTime();
        }
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        
        glfwPollEvents();

        // Debug the execution time to improve optimization
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start); 
        cout << "Execution time of one frame: " << duration.count()/1000.0f << endl; 
    }
    
    glfwTerminate();
    return 0;
}

