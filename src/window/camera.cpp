#include <camera.h>

Camera::Camera() 
{

    Front = glm::vec3(0.0f, 0.0f, -1.0);
    Position = glm::vec3(0.0f, 0.0f, 2.5f); 
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f); 

    MovementSpeed = SPEED/3;
    MouseSensitivity = SENSITIVITY;
    Zoom = ZOOM;
    
    updateCameraVectors();

    Position -= Right * 30.0f * MovementSpeed*1.2f;
    updateCameraVectors();

}

void Camera::defMonitor()
{
    monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);
}
// Configure transformation matrices
glm::mat4 Camera::GetProjectionMatrix()
{
     return glm::perspective(glm::radians(Zoom), (float)mode->width/mode->height, 0.001f, 100.0f);
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction)
{
    float velocity = MovementSpeed;
    if (direction == UP)
        Position += Up * velocity;
    if (direction == DOWN)
        Position -= Up * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{       
    Position += Front*yoffset/(20.0f);
    if (Position.z < 0.001f)
        Position.z = 0.001f;
    if (Position.z > 200.0f)
        Position.z = 200.0f;  
}

void Camera::updateCameraVectors()
{

    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}