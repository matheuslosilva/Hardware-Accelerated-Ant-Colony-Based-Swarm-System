#include <openglContext.h>

#define SIMULATION_PERIOD 15000

OpenglContext::OpenglContext(){}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

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

    swarmSimulatorWindow = window;

    camera = new Camera();
    camera->defMonitor();

    // glfw window creation
    // --------------------
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    userInterface = new UI();
    userInterface->init(swarmSimulatorWindow);

    openGlRenderUpdateFrameRate = 1;

    AdditionalParameters = (AdditionalCallbackParameters*) malloc(sizeof(AdditionalParameters));
    AdditionalParameters->camera = camera;
    AdditionalParameters->window = window;
    AdditionalParameters->userInterface = userInterface;

    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(AdditionalParameters));
}

void OpenglContext::pollEvents()
{
    glfwPollEvents();

    frameCounter++;
    frameCounter %= (SIMULATION_PERIOD + 1); 
    //frameCounter %= 1000; 
}

void OpenglContext::pre_render()
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void OpenglContext::post_render()
{
    glfwSwapBuffers(swarmSimulatorWindow);
}

void OpenglContext::end()
{
    glfwDestroyWindow(swarmSimulatorWindow);
    glfwTerminate();
}

void OpenglContext::run(OpenglBuffersManager* openglBuffersManager)
{
    // render loop
    // -------------------------------------------------------------------
    while (userInterface->stateSimulation != CLOSED && !glfwWindowShouldClose(swarmSimulatorWindow))
    {
        switch (userInterface->stateSimulation)
        {
            case RESET:

                environment->resetEnvironment();
                openglBuffersManager->resetBufferManager();
                pre_render();  

                while (userInterface->stateSimulation == RESET)
                {
                    pollEvents(); // Framecounter++ here
                               
                    switch (userInterface->UIAction)
                    {   
                        case ENVIRONMENT_INIT:
                            parameterAssigner = new ParameterAssigner("experiments/experiment.json");
                            environment = new Environment(parameterAssigner);
                            environment->initializeEnvironment(openglBuffersManager);
                            environment->draw(openglBuffersManager, camera);
                        break;
                        case ADD_NEST:
                            environment->createNest(0, openglBuffersManager);
                            userInterface->UIAction = DO_NOTHING;  
                        break; 
                        
                        case ADD_FOOD:
                            environment->createFoodSource(0, openglBuffersManager);
                            userInterface->UIAction = DO_NOTHING;  
                        break; 

                        case ADD_ANT:
                            environment->createAnt(userInterface->nestID, openglBuffersManager);
                            userInterface->UIAction = DO_NOTHING;  
                        break;
                    }
                    
                    userInterface->run();
                    environment->draw(openglBuffersManager, camera);
                    post_render();
                }

            break;

            case RUNNING:

                while (userInterface->stateSimulation == RUNNING)
                {                    
                    pollEvents();// Framecounter++ here

                    environment->run(frameCounter);

                    if(userInterface->turnOnGraphics && ((frameCounter % openGlRenderUpdateFrameRate) == 0)) 
                    { 
                        openGlRenderUpdateFrameRate = userInterface->openGlRenderUpdateFrameRate;
                        pre_render();
                        environment->draw(openglBuffersManager, camera);
                    }
                    userInterface->run(); //RETIRAR DAQUI PARA MAIOR EXCLUSIVIDADE DO RUN
                    post_render(); 

                    if (frameCounter == SIMULATION_PERIOD) {
                        userInterface->stateSimulation = CLOSED;
                        std::cout << environment->nests[0]->foodAmount;
                    }

                    //debugExecutionTimeStart();
                    //debugExecutionTimeStop("total");
                    //usleep(0.01 * microsecond);
                }   

            break;

            case PAUSED:
                parameterAssigner = new ParameterAssigner("src/swarmEnvironment/experiments/experiment.json");
                environment = new Environment(parameterAssigner);
                environment->initializeEnvironment(openglBuffersManager);
                
                environment->createNest(0, openglBuffersManager);
                environment->createFoodSource(0, openglBuffersManager);
                environment->createFoodSource(1, openglBuffersManager);
                environment->createFoodSource(2, openglBuffersManager);
                environment->createFoodSource(3, openglBuffersManager);
                environment->createAnt(0, openglBuffersManager);
                while (userInterface->stateSimulation == PAUSED)
                {
                    pollEvents();

                    pre_render();
                    environment->draw(openglBuffersManager, camera);

                    userInterface->run();
                    post_render();

                    
                }

            break;
        }
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    AdditionalCallbackParameters *AdditionalParameters = reinterpret_cast<AdditionalCallbackParameters *>(glfwGetWindowUserPointer(window));
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        AdditionalParameters->userInterface->stateSimulation = CLOSED;

    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
        AdditionalParameters->camera->ProcessKeyboard(UP);

    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
        AdditionalParameters->camera->ProcessKeyboard(DOWN);

    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
        AdditionalParameters->camera->ProcessKeyboard(LEFT);

    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
        AdditionalParameters->camera->ProcessKeyboard(RIGHT);
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    AdditionalCallbackParameters *AdditionalParameters = reinterpret_cast<AdditionalCallbackParameters *>(glfwGetWindowUserPointer(window));
    
    AdditionalParameters->camera->ProcessMouseScroll(yoffset);
}

static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    glfwGetCursorPos(window, &xpos, &ypos);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    //if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
}