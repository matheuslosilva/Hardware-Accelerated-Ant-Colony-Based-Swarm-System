#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>

class EBO
{
	public:
		// ID reference of Elements Buffer Object
		
		GLuint ID;
		// Constructor that generates a Elements Buffer Object and links it to indices
		EBO(GLsizeiptr size, GLuint* indices);

		void bind();
		void unbind();
		void erase();
};

#endif