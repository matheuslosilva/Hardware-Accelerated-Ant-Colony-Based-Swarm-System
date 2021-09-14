#ifndef GLFWENVIRONMENT_H
#define GLFWENVIRONMENT_H

#include <constants.h>

GLFWwindow*  glfwInitialize();
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


#endif
