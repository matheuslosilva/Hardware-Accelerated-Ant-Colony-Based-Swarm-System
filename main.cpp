#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <bitset>
#include<vector>

#include <shader.h>
#include <EBO.h>
#include <VAO.h>
#include <VBO.h>

#include <chrono>

using namespace std::chrono;
using namespace std;

#include <ant.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH  = 1000;
const unsigned int SCR_HEIGHT = 1000;
const unsigned int POP_SIZE   = 300000;
const int    CHANNEL_COUNT    = 4;
const GLenum PIXEL_FORMAT     = GL_RGBA;
const int    DATA_SIZE        = SCR_WIDTH * SCR_HEIGHT * CHANNEL_COUNT;


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

    bitset<32> r2 = (bitset<32>)actualColor & (bitset<32>)255;
    bitset<32> g2 = 0;
    bitset<32> b2 = (bitset<32>)actualColor >> 16 & (bitset<32>) 255;
    bitset<32> a2 = 0;

    int r2i = (int)r2.to_ulong();
    r2i += 20;
    r2 = (bitset<32>) r2i;

    bitset<32> rgba = ((a2<<24)|(b2<<16)|(g2<<8)|r2);

    ptr[(((int)yn)*SCR_WIDTH+(int)xn)] = (GLbitfield)rgba.to_ulong();
}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shaderAnts("shaders/vertexShader.s", "shaders/fragmentShader.s");

    float Vertices[] = {
        // positions
         0.0f,  0.025f,
        -0.05f, -0.05f,
         0.05f, -0.05f
    };
    VBO VBOPositions(Vertices, sizeof(Vertices), GL_STATIC_DRAW);

    float Colors[] = {
        // positions
         1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f
    };
    VBO VBOColors(Colors, sizeof(Colors), GL_STATIC_DRAW);

    vector<Ant*> antsColony;
    for(int i = 0; i < POP_SIZE; i++)
    {
        antsColony.push_back(new Ant(0, 0, (glm::radians(360.0f)/(float)POP_SIZE)*i, 0.05f, 0.0001f, 10.0f));
    }

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
    VBO VBOTranformations(&modelMatrices[0], (sizeof(glm::mat4) * POP_SIZE), GL_DYNAMIC_DRAW);

    // Generates Vertex Array Object and binds it
    VAO VAOAnts;
    VAOAnts.Bind();

    VAOAnts.LinkVBO(VBOPositions, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
    VAOAnts.LinkVBO(VBOColors, 1, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    VAOAnts.LinkVBOMatrix4(VBOTranformations, 2, 4, GL_FLOAT, sizeof(glm::mat4));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    VAOAnts.Unbind();

    Shader shaderPheromone("shaders/texVertexShader.s", "shaders/textureShader.s");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float VerticesTexture[] = {
        // positions               // texture coords
        -1.0f,  1.0f, 0.0f,     0.0f, 1.0f,  // top left
        1.0f,  1.0f, 0.0f,      1.0f, 1.0f, // top right
        1.0f,  -1.0f, 0.0f,     1.0f, 0.0f, // bottom right
        -1.0f,  -1.0f, 0.0f,     0.0f, 0.0f // bottom left
    };
    VBO VBOVerticesTexture(VerticesTexture, sizeof(VerticesTexture), GL_STATIC_DRAW);

    unsigned int indicesTexture[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    EBO EBOIndicesTexture(indicesTexture, sizeof(indicesTexture));

    VAO VAOPheromone;
    VAOPheromone.Bind();

    EBOIndicesTexture.Bind();
    VAOPheromone.LinkVBO(VBOVerticesTexture, 0, 3, GL_FLOAT, 5* sizeof(float), (void*)0);
    VAOPheromone.LinkVBO(VBOVerticesTexture, 2, 2, GL_FLOAT, 5* sizeof(float), (void*)(3 * sizeof(float)));
    VAOPheromone.Unbind();

    // global variables
    GLuint pboIds[2];                   // IDs of PBO
    GLuint textureId;                   // ID of texture
    GLubyte* imageData = 0;

    // allocate texture buffer
    imageData = new GLubyte[DATA_SIZE];
    memset(imageData, 0, DATA_SIZE);

    vector<int> pheromoneMatrix(DATA_SIZE/4);

    // init 2 texture objects
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
    glBindTexture(GL_TEXTURE_2D, 0);

    // create 2 pixel buffer objects, you need to delete them when program exits.
    // glBufferData() with NULL pointer reserves only memory space.
    glGenBuffers(2, pboIds);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[0]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[1]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    int l= 0;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        l++;
        auto start = high_resolution_clock::now();
        
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //----------------------------------------------------------------
        shaderAnts.use();
        VAOAnts.Bind();
        VBOTranformations.Bind();
        for(int i = 0; i < POP_SIZE; i++)
        {        
            float oldtheta = antsColony[i]->_theta;
            antsColony[i]->move(l);
            modelMatrices[i] = glm::rotate(modelMatrices[i], antsColony[i]->_theta - oldtheta, glm::vec3(0.0f, 0.0f, 1.0f));
            modelMatrices[i][3][0] = (float)antsColony[i]->_x;
            modelMatrices[i][3][1] = (float)antsColony[i]->_y;
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4)*POP_SIZE, modelMatrices);
        VBOTranformations.Unbind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, POP_SIZE); // 100000 triangles of 6 vertices each
        VAOAnts.Unbind();
        //----------------------------------------------------------------


        static int index = 0;
        int nextIndex = 0;
        // In dual PBO mode, increment current index first then get the next index
        index = (index + 1) % 2;
        nextIndex = (index + 1) % 2;


        //----------------------------------------------------------------
        // bind PBO to update pixel values
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[nextIndex]);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
        GLbitfield* ptr = (GLbitfield*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
        double xpos, ypos;

        if(l%20==0)
        { // update data directly on the mapped buffer
            for(int i = 0; i < POP_SIZE; i++)
            {
                //cout << "x: " << antsColony[i]->_x << " Y: " << antsColony[i]->_y << endl;
                updatePixels(pheromoneMatrix, antsColony[i]->_x, antsColony[i]->_y, i);
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
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[index]);
        // copy pixels from PBO to texture object
        // Use offset instead of ponter.
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT, PIXEL_FORMAT, GL_UNSIGNED_BYTE, 0);
        //----------------------------------------------------------------
        glBindTexture(GL_TEXTURE_2D, 0);


        glBindTexture(GL_TEXTURE_2D, textureId);
        // render container
        shaderPheromone.use();


        VAOPheromone.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        VAOPheromone.Unbind();
        glBindTexture(GL_TEXTURE_2D, 0);
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();


        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start); 
        //cout << "Duração total: " << duration.count() << endl; 
    }

    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    VBOTranformations.Delete();
    VBOPositions.Delete();
    VBOColors.Delete();
    VAOAnts.Delete();

    
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

/*  // load and create a texture 
    // -------------------------
    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps

    int width, height, nrChannels;

    unsigned char *data = stbi_load("images.jpeg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
*/
