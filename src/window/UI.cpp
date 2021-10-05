#include <UI.h>


UI::UI()
{
}

void UI::init(GLFWwindow* window)
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::preRender()
{  
    ImGui_ImplOpenGL3_NewFrame();   
    ImGui_ImplGlfw_NewFrame();      
    ImGui::NewFrame();
}

void UI::render()
{

    bool demo = true;  
    ImGui::ShowDemoWindow(&demo); 


    ImGui::Begin("Ant Colony");

    ImGui::Button("Start simulation");

    ImGui::Button("Turn On/Off graphic simulation");

    ImGui::Button("Pause simulation");

    ImGui::Button("Stop simulation");

    ImGui::Button("Place nest");

    ImGui::Button("Place food");

    ImGui::Button("Draw obstacles");

    ImGui::Button("Draw pheromone");

    ImGui::End();
}

void UI::postRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
}

void UI::end()
{
	ImGui_ImplOpenGL3_Shutdown();   
    ImGui_ImplGlfw_Shutdown();      
    ImGui::DestroyContext(); 
}

