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

    //Model nano{ wnd.Gfx(), "Resources\\Models\\nano_textured\\nanosuit.obj", 0.4f };
    //Model wall{ wnd.Gfx(),"Resources\\Models\\brick_wall\\brick_wall.obj", 2.0f};
    //Model goblin{ wnd.Gfx(),"Resources\\Models\\Goblin\\GoblinX.obj", 6.0f };

    Model sponza{ wnd.Gfx(),"Resources\\Models\\sponza\\sponza.obj",1.0f / 20.0f };

    TestPlane bluePlane{ wnd.Gfx(),6.0f,{ 0.3f,0.3f,1.0f,0.0f } };
    TestPlane yellowPlane{ wnd.Gfx(),6.0f,{ 1.0f, 1.0f, 0.0f, 0.0f } };
    TestPlane redPlane{ wnd.Gfx(),6.0f,{  1.0f,0.3f,0.3f,0.0f } };

    //TestPlane plane{wnd.Gfx(), 1.0f};

    //TestCube cube;

};