#ifndef UI_H
#define UI_H

#include <constants.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

using namespace std;


#define	DOING_NOTHING 0
#define	PLACING_NEST 1
#define	PLACING_FOOD 2
#define	DRAWING_OBSTACLES 3
#define	DRAWING_PHEROMONE 4


enum StateOfSimulation
{
	STARTED,
	RUNNING,
	PAUSED,
	STOPPED,
	CLOSED
};

class UI 
{
	private:

	public:

		StateOfSimulation stateSimulation;
		int stateOfAction;

		int halfScreenSize;

		bool turnOnGraphics;
    	bool disable_camera_movement;

    	int nOfAnts;
    	int nestSize;
    	int nestPosX;
    	int nestPosY;

    	int foodAmount;
    	int foodSize;
    	int foodPosX;
    	int foodPosY;

		float mouseX, mouseY;

		UI();

		void init(GLFWwindow* window);
		void pre_render();
		void render();
		void post_render();
		void run();
		void end();

};
#endif