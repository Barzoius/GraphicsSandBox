#pragma once

#include "Window.h"
#include "Timer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Mesh.h"

#include "TestPlane.h"
//#include "TestCube.h"

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

    /*Model nano{ wnd.Gfx(), "Resources\\Models\\nano_textured\\nanosuit.obj" };
    Model nano2{ wnd.Gfx(), "Resources\\Models\\nano_textured\\nanosuit.obj" };

    TestPlane plane;*/

    //Model wall{ wnd.Gfx(),"Resources\\Models\\brick_wall\\brick_wall.obj" };
    Model goblin{ wnd.Gfx(),"Resources\\Models\\Goblin\\GoblinX.obj" };
  
    //TestCube cube;

};