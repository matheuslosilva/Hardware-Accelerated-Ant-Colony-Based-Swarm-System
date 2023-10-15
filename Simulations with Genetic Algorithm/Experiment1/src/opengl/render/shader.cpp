#include "shader.h"

using namespace std;

Shader::Shader()
{
}

string Shader::extractText(const char* filePath)
{
	string shaderCode;
    ifstream shaderFile;

	shaderFile.exceptions (ifstream::failbit | ifstream::badbit);
	try
	{
	    shaderFile.open(filePath);

	    stringstream shaderString;

	    shaderString << shaderFile.rdbuf();
	    
	    shaderFile.close();

	    shaderCode = shaderString.str();
	}
	catch (ifstream::failure& e)
    {
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
	
	return shaderCode;	
}

GLuint Shader::loadShader(GLenum type, const char* filePath)
{	
	string shaderCode = extractText(filePath);

	GLuint newShader;
	newShader = glCreateShader(type);

	const char* constShaderCode = shaderCode.c_str();
    glShaderSource(newShader, 1, &constShaderCode, NULL);

    glCompileShader(newShader);

    checkErrors(newShader, "COMPILATION");

	return newShader;
}

void Shader::createProgramLinkShaders(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const char* pixelPath)
{
	programID = glCreateProgram();

	if(vertexPath != NULL)
	{
		vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);
		glAttachShader(programID, vertexShader);
	}
	if(fragmentPath != NULL)
	{
		fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);
		glAttachShader(programID, fragmentShader);
	}
	if(geometryPath != NULL)
	{
		geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryPath);
		glAttachShader(programID, geometryShader);
	}

	glLinkProgram(programID);

	checkErrors(programID, "PROGRAM");

	glDetachShader(programID, vertexShader);
	glDetachShader(programID, fragmentShader);
	glDetachShader(programID, geometryShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);
}

void Shader::bind() const
{
	glUseProgram(programID);
}

void Shader::unBind() const
{
	glUseProgram(0);
}

GLuint Shader::getProgramID() const
{ 
	return programID; 
}

void Shader::dispose()
{
	glDeleteProgram(programID);
	programID = -1;
}

void Shader::checkErrors(GLuint shaderObj, std::string type)
{
    GLint status;
    GLchar infoLog[1024];
    if(type == "PROGRAM")
    {
    	glGetProgramiv(shaderObj, GL_LINK_STATUS, &status);
        if(!status)
        {
            glGetProgramInfoLog(shaderObj, 1024, NULL, infoLog);
            std::cout << "PROGRAM LINK ERROR: " << "\n" << infoLog << "\n" << std::endl;
        }
    }
    else
    {
        glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &status);
        if(!status)
        {
            glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
            std::cout << "SHADER COMPILATION ERROR: " << type << "\n" << infoLog << "\n" << std::endl;
        }
    }
}

// SET SHADER ATTRIBUTES
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

