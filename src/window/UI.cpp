#include <UI.h>

UI::UI()
{
    stateSimulation = STARTED;
    turnOnGraphics = true;
}

void UI::init(GLFWwindow* window)
{
    halfScreenSize = SCR_WIDTH/2;
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::pre_render()
{  
    ImGui_ImplOpenGL3_NewFrame();   
    ImGui_ImplGlfw_NewFrame();      
    ImGui::NewFrame();
}

void UI::render()
{
    ImGui::Begin("Ant Colony", NULL, ImGuiWindowFlags_NoMove);

    ImGuiIO& io = ImGui::GetIO();

    mouseX = io.MousePos.x;
    mouseY = io.MousePos.y;

    if(ImGui::Button("Start simulation")) stateSimulation = RUNNING;

    if(ImGui::Button("Pause simulation")) stateSimulation = PAUSED;

    if(ImGui::Button("Stop simulation")) stateSimulation = STOPPED;

    if(ImGui::Button("Close simulation")) stateSimulation = CLOSED;

    ImGui::Text("\n");
    

    ImGui::Checkbox("Turn On graphic simulation", &turnOnGraphics);
    //ImGui::Checkbox("Disable Camera movement", &disable_camera_movement);

    ImGui::Text("\n");  


    ImGui::SliderInt("Number of ants", &nOfAnts, 10, 300000);
    ImGui::SliderInt("Nest size", &nestSize, -20, 20);
    ImGui::SliderInt("Nest posX", &nestPosX, -halfScreenSize, halfScreenSize);
    ImGui::SliderInt("Nest posY", &nestPosY, -halfScreenSize, halfScreenSize);
    if(ImGui::Button("Place nest")) stateOfAction = PLACING_NEST;

    ImGui::Text("\n");

    ImGui::SliderInt("Food amount", &foodAmount, 10, 1000000);
    ImGui::SliderInt("Food size", &foodSize, -20, 20);
    ImGui::SliderInt("Food posX", &foodPosX, -halfScreenSize, halfScreenSize);
    ImGui::SliderInt("Food posY", &foodPosY, -halfScreenSize, halfScreenSize);
    if(ImGui::Button("Place food")) stateOfAction = PLACING_FOOD;

    ImGui::Text("\n");

    //ImGui::RadioButton("Draw obstacles", &stateOfAction, DRAWING_OBSTACLES);
    //ImGui::RadioButton("Draw pheromone", &stateOfAction, DRAWING_PHEROMONE);

    ImGui::End();
}

void UI::post_render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
}

void UI::run()
{
    pre_render();
    render(); 
    post_render();
}

void UI::end()
{
	ImGui_ImplOpenGL3_Shutdown();   
    ImGui_ImplGlfw_Shutdown();      
    ImGui::DestroyContext(); 
}

