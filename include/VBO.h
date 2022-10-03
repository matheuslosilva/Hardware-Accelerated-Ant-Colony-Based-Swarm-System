#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class VBO
{

	public:
		// Reference ID of the Vertex Buffer Object
		GLuint ID;

		// Constructor that generates a Vertex Buffer Object and links it to vertices
		VBO(GLsizeiptr size, void* data, GLenum usage);
		void resizeBuffer(GLsizeiptr size, void* data, GLenum usage);
		void subBufferData(GLsizeiptr size, void* data);
		void bind();
		void unbind();
		void erase();
};

#endif
