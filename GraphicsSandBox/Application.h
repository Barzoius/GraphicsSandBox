#pragma once

#include "Window.h"
#include "Timer.h"
#include "ImGuiManager.h"
#include "Camera.h"


#include "PointLight.h"

class Application
{
public:
    
    Application();

    ~Application();

    int Run();


private:

    void DoFrame();

private:
    ImGuiManager imgui;
    Window wnd;
    Timer timer;
    Camera camera;
    PointLight light;

    float speedFactor = 1.0f;
    bool show_imgui_demo = true;

    std::unique_ptr<class Cuboid> testCube;
    std::unique_ptr<class Cuboid> testUVShhere;
    std::unique_ptr<class Cuboid> testSheet;

    std::vector<std::unique_ptr<class Cuboid>> cuboids;

    std::vector<std::unique_ptr<class Drawable>> drawables;
    static constexpr size_t nDrawables = 180;
};