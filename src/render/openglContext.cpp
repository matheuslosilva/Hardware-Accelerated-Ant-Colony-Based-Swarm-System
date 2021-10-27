#include <openglContext.h>


using namespace std::chrono;

OpenglContext::OpenglContext(){}


void OpenglContext::debugExecutionTimeStart()
{
    startDebugTimer = std::chrono::high_resolution_clock::now(); // Execution time counte
}

void OpenglContext::debugExecutionTimeStop(std::string debugText)
{
    // Debug the execution time to improve optimization
    stopDebugTimer = high_resolution_clock::now(); 
    durationTimer = duration_cast<microseconds>(stopDebugTimer - startDebugTimer); 
    cout << "Execution time of "<< debugText << ": " << durationTimer.count()/1000.0f << endl; 
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void OpenglContext::processInput(UI* userInterface)
{
    frameCounter++;
    frameCounter %= 1000;

    if (glfwGetKey(antColonyWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(antColonyWindow, true);

    if (glfwGetKey(antColonyWindow, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(UP);

    if (glfwGetKey(antColonyWindow, GLFW_KEY_S) == GLFW_PRESS)
       camera->ProcessKeyboard(DOWN);

    if (glfwGetKey(antColonyWindow, GLFW_KEY_A) == GLFW_PRESS)
       camera->ProcessKeyboard(LEFT);
    
    if (glfwGetKey(antColonyWindow, GLFW_KEY_D) == GLFW_PRESS)
       camera->ProcessKeyboard(RIGHT); 
    
    mouseState = glfwGetMouseButton(antColonyWindow, GLFW_MOUSE_BUTTON_LEFT);
    glfwPollEvents();
    
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(yoffset);
}


void  OpenglContext::init()
{
	// glfw: initialize and configure
    // ------------------------------
    glfwInit();
   
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Swarm Simulator", monitor, NULL);
    glfwSetWindowMonitor(window, NULL, 1, 1, mode->width, mode->height, mode->refreshRate);


    // glfw window creation
    // --------------------
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scroll_callback);
 

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    antColonyWindow = window;
}

void OpenglContext::pre_render()
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
}

void OpenglContext::post_render()
{
    glfwSwapBuffers(antColonyWindow);
}

void OpenglContext::end()
{
    glfwDestroyWindow(antColonyWindow);
    glfwTerminate();
}
