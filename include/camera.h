#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <chrono>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
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


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
    public:

        GLFWmonitor* monitor;
        const GLFWvidmode* mode;

        // camera Attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        // camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

         // constructor with vectors
        Camera();
    
        
        void defMonitor();

        // Configure transformation matrices
        glm::mat4 GetProjectionMatrix();

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 GetViewMatrix();

        // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(Camera_Movement direction);    

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset);
       

    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors();
      
};
#endif