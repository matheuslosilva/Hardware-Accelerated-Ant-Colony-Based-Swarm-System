#include <environment.h>

#include <chrono>

using namespace std::chrono;
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// copy an image data to texture buffer
///////////////////////////////////////////////////////////////////////////////
void updatePixels(vector<int> &dst, float x, float y, int numeroformiga)
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

    vector<int> pheromoneMatrix(DATA_SIZE/4);
   
    int frameCounter= 0;
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
       

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Configure transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        
        //----------------------------------------------------------------

        // Ants
        //----------------------------------------------------------------
        antColony.moveAnts(frameCounter);

        shaderAnts.use();
        shaderAnts.setMat4("view", view);
        shaderAnts.setMat4("projection", projection);

        VAOAnts.Bind();

        AntsMatricesBuffer.Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4)*POP_SIZE, antColony.antsModelMatrices);
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
        GLbitfield* ptr = (GLbitfield*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
        double xpos, ypos;

        if(frameCounter%1==0)
        { // update data directly on the mapped buffer
            for(int i = 0; i < POP_SIZE; i++)
            {
                //cout << "x: " << antsColony[i]->_x << " Y: " << antsColony[i]->_y << endl;
                updatePixels(pheromoneMatrix, antColony.ants[i]->_x, antColony.ants[i]->_y, i);
                //updatePixels(ptr, xpos, ypos);
            }
        }
        memcpy(ptr, pheromoneMatrix.data(), DATA_SIZE);
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // release pointer to mapping buffer
        // It is good idea to release PBOs with ID 0 after use.
        // Once bound with 0, all pixel operations behave normal ways.
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        //----------------------------------------------------------------

        //----------------------------------------------------------------
        // bind Texture
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[indexPBO]);
        // copy pixels from PBO to texture object
        // Use offset instead of ponter.
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT, PIXEL_FORMAT, GL_UNSIGNED_BYTE, 0);
        //----------------------------------------------------------------
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        
        // render container
        shaderPheromone.use();
        shaderPheromone.setMat4("view", view);
        shaderPheromone.setMat4("projection", projection);

        VAOPheromone.Bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        VAOPheromone.Unbind();
        //--------------------------------------------------------------------------------



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Debug the execution time to improve optimization
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start); 
        cout << "Execution time of one frame: " << duration.count() << endl; 
    }
    
    glfwTerminate();
    return 0;
}

