#ifndef VAO_H
#define VAO_H

#include<glad/glad.h>
#include"VBO.h"
#include<string>

class VAO
{
	public:
		// ID reference for the Vertex Array Object
		GLuint ID;
		// Constructor that generates a VAO ID
		VAO();

		// Links a VBO to the VAO using a certain layout
		void LinkVBO(VBO& VBO, GLuint layout, GLuint size, GLenum type, GLsizei stride, GLvoid* pointer);
		void LinkVBOMatrix4(VBO& VBO, GLuint layout, GLuint size, GLenum type, GLsizei stride);
		// Binds the VAO
		// -----------------------------------------------------------------------
		void Bind();
		// Unbinds the VAO
		void Unbind();
		// Deletes the VAO
		void Delete();

	
};
#endif
