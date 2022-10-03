#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

enum Camera_Movement {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Default camera values
const float SPEED       =  0.01f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
    public:

        GLFWmonitor* monitor;
        const GLFWvidmode* mode;

        // camera Attributes
        glm::vec3 Position;
        glm::vec3 PointsTo;
        glm::vec3 WorldUp;

        glm::vec3 Up;
        glm::vec3 Right;
    
        // camera options
        float MovementSpeed;
        float Zoom;

         // constructor with vectors
        Camera();
        
        void defMonitor();

        glm::mat4 GetProjectionMatrix();
        glm::mat4 GetViewMatrix();
        void ProcessKeyboard(Camera_Movement direction);    
        void ProcessMouseScroll(float yoffset);
      
};
#endif