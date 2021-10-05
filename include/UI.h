#ifndef UI_H
#define UI_H

#include <constants.h>

using namespace std;

class UI 
{
	private:
		

	public:
		UI();

		void init(GLFWwindow* window);
		void preRender();
		void render();
		void postRender();
		void end();

};
#endif