#include"VAO.h"

// Constructor that generates a VAO ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
	glBindVertexArray(0);
}

// Links a VBO to the VAO 
void VAO::linkVBO(VBO& VBO, GLuint index, GLuint size, GLenum type, GLsizei stride, GLvoid* offset)
{
	VBO.bind();

	glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(index);

	VBO.unbind();

}

void VAO::linkVBOMatrix4(VBO& VBO, GLuint index, GLuint size, GLenum type, GLsizei stride)
{
	VBO.bind();

	glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void*)(0 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(index+1);
    glVertexAttribPointer(index+1, size, type, GL_FALSE, stride, (void*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(index+2);
    glVertexAttribPointer(index+2, size, type, GL_FALSE, stride, (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(index+3);
    glVertexAttribPointer(index+3, size, type, GL_FALSE, stride, (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

	VBO.unbind();	
}


// Binds the VAO
void VAO::bind()
{
	glBindVertexArray(ID);
}

// Unbinds the VAO
void VAO::unbind()
{
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO::erase()
{
	glDeleteVertexArrays(1, &ID);
}