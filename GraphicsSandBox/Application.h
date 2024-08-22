#pragma once

#include "Window.h"
#include "Timer.h"
#include "ImGuiManager.h"
#include "Camera.h"

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

    float speedFactor = 1.0f;
    bool show_imgui_demo = true;

    std::vector<std::unique_ptr<class Cuboid>> cuboids;

    std::vector<std::unique_ptr<class Drawable>> drawables;
    static constexpr size_t nDrawables = 180;
};