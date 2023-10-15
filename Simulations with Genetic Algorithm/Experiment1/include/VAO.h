#ifndef VAO_H
#define VAO_H

#include <VBO.h>
#include <EBO.h>
#include <string>

class VAO
{
	public:
		// ID reference for the Vertex Array Object
		GLuint ID;
		// Constructor that generates a VAO ID
		VAO();

		// Links a VBO to the VAO using a certain layout
		void linkVBO(VBO& VBO, GLuint layout, GLuint size, GLenum type, GLsizei stride, GLvoid* offset);
		void linkVBOMatrix4(VBO& VBO, GLuint layout, GLuint size, GLenum type, GLsizei stride);
		
		void bind();
		void unbind();
		void erase();
};
#endif
