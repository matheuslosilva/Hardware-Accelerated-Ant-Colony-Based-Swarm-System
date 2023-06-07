#include <camera.h>

Camera::Camera() 
{
    Position = glm::vec3(0.008f, 0.0f,  1.5f); 
    PointsTo = glm::vec3(0.0f, 0.0f, -1.0f);
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f); 

    MovementSpeed = SPEED*4;
    Zoom = ZOOM;
    
    Right = glm::normalize(glm::cross(PointsTo, WorldUp));
    Up    = glm::normalize(glm::cross(Right, PointsTo));

}

// Configure transformation matrices
glm::mat4 Camera::GetProjectionMatrix()
{
     return glm::perspective(glm::radians(Zoom), (float)mode->width/mode->height, 0.001f, 100.0f);
}

// returns the view matrix calculated using Euler Angles and the PointsTo Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + PointsTo, WorldUp);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction)
{
    if (direction == UP)
        Position += Up * MovementSpeed;
    if (direction == DOWN)
        Position -= Up * MovementSpeed;
    if (direction == LEFT)
        Position -= Right * MovementSpeed;
    if (direction == RIGHT)
        Position += Right * MovementSpeed;
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{       
    Position += PointsTo * yoffset/15.0f;
    if (Position.z < 0.001f)
        Position.z = 0.001f;
    if (Position.z > 200.0f)
        Position.z = 200.0f;  
}


void Camera::defMonitor()
{
    monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);
}

