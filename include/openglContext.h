#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include <constants.h>
#include <camera.h>

using namespace std::chrono;

class OpenglContext
{
	public:
		GLFWwindow* antColonyWindow;
		unsigned int frameCounter;

		time_point<high_resolution_clock> startDebugTimer;
	    time_point<high_resolution_clock> stopDebugTimer; 
	    microseconds durationTimer; 

		OpenglContext();

		void debugExecutionTimeStart();

		void debugExecutionTimeStop(std::string debugText);


		void  init();
		void processInput(Camera* camera);
		void pre_render();
		void post_render();
		void end();
};



#endif
