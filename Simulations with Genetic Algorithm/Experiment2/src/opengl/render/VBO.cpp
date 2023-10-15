#include"VBO.h"


// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(GLsizeiptr size, void* data, GLenum usage)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::resizeBuffer(GLsizeiptr size, void* data, GLenum usage)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::subBufferData(GLsizeiptr size, void* data)
{
 	glBindBuffer(GL_ARRAY_BUFFER, ID);
   	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Binds the VBO
void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::erase()
{
	glDeleteBuffers(1, &ID);
}