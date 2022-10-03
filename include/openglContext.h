#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include <openglBuffersManager.h>

#include <environment.h>

typedef struct
{
	Camera *camera;
    GLFWwindow *window;
    UI* userInterface;
}AdditionalCallbackParameters;

class OpenglContext
{
	public:
		AdditionalCallbackParameters* AdditionalParameters;

		GLFWwindow* swarmSimulatorWindow;
		Camera* camera;
		UI* userInterface;
		Environment* environment;
		ParameterAssigner* parameterAssigner;

		unsigned int frameCounter;
		int openGlRenderUpdateFrameRate;

		OpenglContext();
		void  init();
		void pollEvents();
		void run(OpenglBuffersManager* openglBuffersManager);
		void pre_render();
		void post_render();
		void end();
};


#endif
