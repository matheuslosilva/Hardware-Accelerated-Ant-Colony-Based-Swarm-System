#include"VAO.h"

// Constructor that generates a VAO ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Links a VBO to the VAO 
void VAO::LinkVBO(VBO& VBO, GLuint layout, GLuint size, GLenum type, GLsizei stride, GLvoid* pointer)
{
	VBO.Bind();

	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, size, type, GL_FALSE, stride, pointer);

	VBO.Unbind();

}

void VAO::LinkVBOMatrix4(VBO& VBO, GLuint layout, GLuint size, GLenum type, GLsizei stride)
{
	VBO.Bind();

	glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, size, type, GL_FALSE, stride, (void*)(0 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(layout+1);
    glVertexAttribPointer(layout+1, size, type, GL_FALSE, stride, (void*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(layout+2);
    glVertexAttribPointer(layout+2, size, type, GL_FALSE, stride, (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(layout+3);
    glVertexAttribPointer(layout+3, size, type, GL_FALSE, stride, (void*)(3 * sizeof(glm::vec4)));

	VBO.Unbind();	
}


// Binds the VAO
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// Unbinds the VAO
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}