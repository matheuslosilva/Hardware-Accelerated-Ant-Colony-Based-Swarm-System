#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <chrono>
#include <iostream>
#include <constants.h>
#include <UI.h>


using namespace std::chrono;

class OpenglContext
{
	public:
		GLFWwindow* antColonyWindow;
		unsigned int frameCounter;
		int mouseState;
		
		time_point<high_resolution_clock> startDebugTimer;
	    time_point<high_resolution_clock> stopDebugTimer; 
	    microseconds durationTimer; 


		OpenglContext();

		void debugExecutionTimeStart();

		void debugExecutionTimeStop(std::string debugText);


		void  init();
		void processInput(UI* userInterface);
		void pre_render();
		void post_render();
		void end();
};



#endif
