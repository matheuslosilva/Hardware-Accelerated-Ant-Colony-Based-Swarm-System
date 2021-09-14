#include <manageGlBindings.h>


VBO createAntsVertexObjects(VAO VAOAnts, glm::mat4* antsModelMatrices)
{
    float Vertices[] = {
        // positions
         0.0f,  0.025f,
        -0.05f, -0.05f,
         0.05f, -0.05f
    };
    VBO VBOPositions(Vertices, sizeof(Vertices), GL_STATIC_DRAW);

    float Colors[] = {
        // positions
         0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f
    };
    VBO VBOColors(Colors, sizeof(Colors), GL_STATIC_DRAW);


    VBO VBOTranformations(&antsModelMatrices[0], (sizeof(glm::mat4) * POP_SIZE), GL_DYNAMIC_DRAW);

    // Generates Vertex Array Object and binds it
    VAOAnts.Bind();

    VAOAnts.LinkVBO(VBOPositions, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
    VAOAnts.LinkVBO(VBOColors, 1, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    VAOAnts.LinkVBOMatrix4(VBOTranformations, 2, 4, GL_FLOAT, sizeof(glm::mat4));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    VAOAnts.Unbind();

    return VBOTranformations;
}

void createPixelBuffers()
{
    GLubyte* imageData = 0;
    // allocate texture buffer
    imageData = new GLubyte[DATA_SIZE];
    memset(imageData, 0, DATA_SIZE);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create two pixel buffers
    glGenBuffers(2, pboIds);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[0]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds[1]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}


VAO createPheromoneVertexObjects()
{
    VAO VAOPheromone;

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

    VAOPheromone.Bind();
    EBOIndicesTexture.Bind();
    VAOPheromone.LinkVBO(VBOVerticesTexture, 0, 3, GL_FLOAT, 5* sizeof(float), (void*)0);
    VAOPheromone.LinkVBO(VBOVerticesTexture, 2, 2, GL_FLOAT, 5* sizeof(float), (void*)(3 * sizeof(float)));
    VAOPheromone.Unbind();

    createPixelBuffers();
    return VAOPheromone;
}

