#pragma once

#include "Window.h"
#include "Timer.h"
#include "ImGuiManager.h"

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

    std::vector<std::unique_ptr<class Cuboid>> cuboids;

    std::vector<std::unique_ptr<class Drawable>> drawables;
    static constexpr size_t nDrawables = 180;
};