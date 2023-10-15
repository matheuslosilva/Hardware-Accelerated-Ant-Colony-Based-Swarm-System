#include <UI.h>

UI::UI()
{
}

void UI::init(GLFWwindow* window)
{
    stateSimulation = PAUSED;
    UIAction = DO_NOTHING;

    turnOnGraphics = false;
    turnOnCameraMovement = true;
    openGlRenderUpdateFrameRate = 1;

    placePheromoneRate = 1;
    pheromoneEvaporationRate = 5;

    nestPosX = 0.0f;
    nestPosY = 0.0f;
    nestSize = 0.04f;
    antAmount = 0;

    foodPosX = 0.0f;
    foodPosY = 0.0f;  
    foodSize = 0.04f; 
    foodAmount = 0;

    nestID = 0;

    halfScreenSize = PIXEL_WIDTH/2;

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    
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
    if (ImGui::BeginMainMenuBar())
    {
        
        if (ImGui::MenuItem("Swarm Type"))
        {
            
        }
        if (ImGui::MenuItem("Environment"))
        {
            
        }
        if (ImGui::MenuItem("Evolutionary Algorithms"))
        {
            
        }
    }
    ImGui::EndMainMenuBar();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    bool open = true;
    ImGui::Begin("EditorDockSpace", &open, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiIO& io = ImGui::GetIO();
        if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("EditorDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        }

        ImGui::Begin("Ant Colony", NULL, NULL);
        simulationControls();
            
        if (ImGui::BeginTabBar("antColonyTabBar", NULL))
        {
            if (ImGui::BeginTabItem("Experiments"))
            {
                experimentsTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Real time interactions"))
            {
                antsRealTimeInteractionsTab();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End(); 

        ImGui::Begin("Log", NULL, NULL);
        ImGui::End();

        ImGui::Begin("Evolutionary Algorithm", NULL, NULL);
        ImGui::End();

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

void UI::simulationControls()
{
    const int intSteps1 = 1;

    ImGui::PushItemWidth(-30);

    if(ImGui::Button("Start simulation")) stateSimulation = RUNNING;
    ImGui::SameLine();
    if(ImGui::Button("Pause simulation")) stateSimulation = PAUSED;

    if(ImGui::Button("Reset simulation")) stateSimulation = RESET;
    ImGui::SameLine();
    if(ImGui::Button("Close simulation")) stateSimulation = CLOSED;

    ImGui::Text("\n");
        
    ImGui::Checkbox("Turn On graphic simulation", &turnOnGraphics);
    ImGui::Checkbox("Disable Camera movement", &turnOnCameraMovement);
    ImGui::Text("OpenGL render rate: "); ImGui::SameLine(); ImGui::InputScalar("##",ImGuiDataType_U32, &openGlRenderUpdateFrameRate, &intSteps1, NULL, "%u");

    ImGui::Text("\n");  
}
void UI::experimentsTab()
{
    static ImVector<int> active_tabs;
    static int next_tab_id = 0;
    /*if (next_tab_id == 0) // Initialize with some default tabs
        for (int i = 0; i < 3; i++)
            active_tabs.push_back(next_tab_id++);*/

    if (ImGui::BeginTabBar("MyTabBar", NULL))
    {
        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
        {
            UIAction = ENVIRONMENT_INIT;
            active_tabs.push_back(next_tab_id++); // Add new tab
        }

        for (int n = 0; n < active_tabs.Size;)
        {
            bool open = true;
            char name[16];
            snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);
            if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
            {
                if(ImGui::Button("Open experiment")) stateSimulation = RUNNING;
                ImGui::SameLine();
                if(ImGui::Button("Save experiment")) stateSimulation = RUNNING;
                if(ImGui::Button("Load experiment")) stateSimulation = RUNNING;

                ImGui::EndTabItem();
            }

            if (!open)
                active_tabs.erase(active_tabs.Data + n);
            else
                n++;
        }
        ImGui::EndTabBar();
    }
}

void UI::antsRealTimeInteractionsTab()
{
    const float floatSteps005 = 0.05f;
    const float floatSteps01 = 0.1f;
    const int intSteps1 = 1;
    const int intSteps100 = 100;
    if (ImGui::BeginTabBar("realTimeInteractionsTabBar", NULL))
    {
        if (ImGui::BeginTabItem("Environment"))
        {
            ImGui::Text("ENVIRONMENT");
            ImGui::Text("Place pheromone rate: "); ImGui::SameLine(); ImGui::InputScalar("a",ImGuiDataType_U32,   &placePheromoneRate, &intSteps1, NULL, "%u");
            ImGui::Text("Pheromone evaporation rate: "); ImGui::SameLine(); ImGui::InputScalar("b",ImGuiDataType_U32, &pheromoneEvaporationRate, &intSteps1, NULL, "%u");

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Nest"))
        {
            ImGui::Text("NEST");
            ImGui::Text("PosX: "); ImGui::SameLine(); ImGui::InputScalar("d", ImGuiDataType_Float,  &nestPosX,  &floatSteps005);
            ImGui::Text("PosY: "); ImGui::SameLine(); ImGui::InputScalar("e", ImGuiDataType_Float,  &nestPosY,  &floatSteps005);
            ImGui::Text("Nest size:"); ImGui::SameLine(); ImGui::InputScalar("f",ImGuiDataType_Float,   &nestSize, &floatSteps01);
            ImGui::Text("Ant amount:"); ImGui::SameLine(); ImGui::InputScalar("g",ImGuiDataType_U32,   &antAmount, &intSteps100, NULL, "%u");
            
            if(ImGui::Button("Add Nest")) UIAction = ADD_NEST;
            ImGui::SameLine(); if(ImGui::Button("Mouse Place Nest")) UIAction = MOUSE_ADD_NEST;

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Ant"))
        {
            ImGui::Text("ANTS");
            ImGui::Text("Nest id: "); ImGui::SameLine(); ImGui::InputScalar("p",ImGuiDataType_U32,   &nestID, &intSteps1, NULL, "%u");
            ImGui::Text("Ant amount:"); ImGui::SameLine(); ImGui::InputScalar("g",ImGuiDataType_U32,   &antAmount, &intSteps100, NULL, "%u");

            if(ImGui::Button("Add Ant")) UIAction = ADD_ANT;
            ImGui::SameLine(); if(ImGui::Button("Mouse Place Ant")) UIAction = MOUSE_ADD_ANT;
            
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Food"))
        {
            ImGui::Text("FOOD");
            ImGui::Text("PosX: "); ImGui::SameLine(); ImGui::InputScalar("h", ImGuiDataType_Float,  &foodPosX,  &floatSteps005);
            ImGui::Text("PosY: "); ImGui::SameLine(); ImGui::InputScalar("i", ImGuiDataType_Float,  &foodPosY,  &floatSteps005);
            ImGui::Text("Food source2 amount:"); ImGui::SameLine(); ImGui::InputScalar("k",ImGuiDataType_U32,   &foodAmount, &intSteps100, NULL, "%u");
            ImGui::Text("Food size:"); ImGui::SameLine(); ImGui::InputScalar("j",ImGuiDataType_Float,   &foodSize, &floatSteps01);

            if(ImGui::Button("Add Food")) UIAction = ADD_FOOD;
            ImGui::SameLine(); if(ImGui::Button("Mouse Place Food")) UIAction = MOUSE_ADD_FOOD;

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}
