#pragma once

#include "Window.h"
#include "Timer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Mesh.h"


#include "PointLight.h"

class Application
{
public:
    
    Application();

    ~Application();

    int Run();


private:

    void DoFrame();

    void ShowRawInputWindow();

private:
    ImGuiManager imgui;
    Window wnd;
    Timer timer;
    Camera camera;
    PointLight light;

    int x = 0;
    int y = 0;

    float speedFactor = 1.0f;
    bool show_imgui_demo = true;

    Model nano{ wnd.Gfx(), "Resources\\Models\\nano_textured\\nanosuit.obj" };

    struct
    {
        float roll = 0.0f;
        float pitch = 0.0f;
        float yaw = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    } pos;
};