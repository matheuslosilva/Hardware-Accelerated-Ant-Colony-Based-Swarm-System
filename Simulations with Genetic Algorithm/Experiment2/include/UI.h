#ifndef UI_H
#define UI_H

#include <constants.h>

#include <iostream>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


using namespace std;

enum Actions
{
	DO_NOTHING,
	ENVIRONMENT_INIT,
	ADD_NEST,
	MOUSE_ADD_NEST,
	ADD_FOOD,
	MOUSE_ADD_FOOD,
	ADD_ANT,
	MOUSE_ADD_ANT,
};

enum StateOfSimulation
{
	RUNNING,
	PAUSED,
	RESET,
	CLOSED
};

class UI 
{
	private:
		

	public:

		StateOfSimulation stateSimulation;
		Actions UIAction;

		int halfScreenSize;

		bool turnOnGraphics;
    	bool turnOnCameraMovement;

    	int placePheromoneRate;
    	int pheromoneEvaporationRate;
    	int openGlRenderUpdateFrameRate;

    	float nestPosX;
    	float nestPosY;
    	float nestSize;
    	int antAmount;

    	float foodPosX;
    	float foodPosY;
    	float foodSize;
    	int foodAmount;

    	int nestID;

		UI();

		void init(GLFWwindow* window);
		void pre_render();
		void render();
		void post_render();
		void run();
		void end();

		void simulationControls();
		void experimentsTab();
		void antsRealTimeInteractionsTab();

};
#endif

