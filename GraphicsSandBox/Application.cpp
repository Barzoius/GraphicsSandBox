    #include "Application.h"

#include "Math.h"
#include "Surface.h"
#include "GDIPManager.h"
#include "Torus.h"

#include "ImGui/imgui.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <sstream>
#include <memory>
#include <algorithm>


#include "VertexSystem.h"
#include "VertexBuffer.h"

GDIPManager gdipm;

using namespace DVS;

Application::Application() 
    : 
    wnd( 1280, 720, "Window" ), 
    light(wnd.Gfx())/*,
    plane(wnd.Gfx(), 3.0f),
    cube(wnd.Gfx(), 3.0f)*/
{
    //wall.SetRootTransform(DirectX::XMMatrixTranslation(-12.0f, 0.0f, 0.0f));
    ////plane.SetPos({ 12.0f,0.0f,0.0f });
    //goblin.SetRootTransform(DirectX::XMMatrixTranslation(0.0f, 0.0f, -4.0f));
    //nano.SetRootTransform(DirectX::XMMatrixTranslation(0.0f, -7.0f, 6.0f));

    bluePlane.SetPos(camera.GetPos());
    redPlane.SetPos(camera.GetPos());
    yellowPlane.SetPos(camera.GetPos());

    wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f,  0.5f, 500.0f));

}

void Application::ShowRawInputWindow()
{

    while(const auto d = wnd.mouse.ReadRawDelta())
    {
        x += d->x;
        y += d->y;
    }

    if (ImGui::Begin("RawInput"))
    {
        ImGui::Text("Tally: (%d,%d)", x, y);
        ImGui::Text("Cursor: %s", wnd.CursorEnabled() ? "enabled" : "disabled");
    }

    ImGui::End();
}

int Application::Run()
{


    while (true)
    {

      if(const auto enter_code = Window::ProccessMessages())
      {
          return *enter_code;
      }
      DoFrame();
      
        
    }

}

Application::~Application()
{}


void Application::DoFrame()
{
    const auto dt = timer.Mark() * speedFactor;

    wnd.Gfx().BeginFrame(0.91f, 0.64f, 0.09f ); //nice yellow
    wnd.Gfx().SetCamera(camera.GetMatrix());

    light.Bind(wnd.Gfx(), camera.GetMatrix());

  

    //const DirectX::XMFLOAT3 material = { 1.0f, 1.0f, 1.0f };
    //auto model = std::make_unique<TestLoadedModel>(wnd.Gfx(), material, 1.5);
    //model->Draw(wnd.Gfx());
    //nano.Draw(wnd.Gfx());

    //wall.Draw(wnd.Gfx());
    ////plane.Draw(wnd.Gfx());

    //goblin.Draw(wnd.Gfx());

    light.Draw(wnd.Gfx());

    sponza.Draw(wnd.Gfx());

    bluePlane.Draw(wnd.Gfx());
    redPlane.Draw(wnd.Gfx());
    yellowPlane.Draw(wnd.Gfx());
    //wall.Draw(wnd.Gfx());

    //cube.Draw(wnd.Gfx());


    while (const auto e = wnd.kbd.ReadKey())
    {
        if (!e->IsPressed())
        {
            continue;
        }

        switch (e->GetCode())
        {
        case VK_ESCAPE:
            if (wnd.CursorEnabled())
            {
                wnd.DisableCursor();
                wnd.mouse.EnableRaw();
            }
            else
            {
                wnd.EnableCursor();
                wnd.mouse.DisableRaw();
            }
            break;
        case VK_F1:
            break;
        }

        if (!wnd.CursorEnabled())
        {
            if (wnd.kbd.KeyIsPressed('W'))
            {
                camera.Translate({ 0.0f,0.0f,dt });
            }
            if (wnd.kbd.KeyIsPressed('A'))
            {
                camera.Translate({ -dt,0.0f,0.0f });
            }
            if (wnd.kbd.KeyIsPressed('S'))
            {
                camera.Translate({ 0.0f,0.0f,-dt });
            }
            if (wnd.kbd.KeyIsPressed('D'))
            {
                camera.Translate({ dt,0.0f,0.0f });
            }
            if (wnd.kbd.KeyIsPressed('R'))
            {
                camera.Translate({ 0.0f,dt,0.0f });
            }
            if (wnd.kbd.KeyIsPressed('F'))
            {
                camera.Translate({ 0.0f,-dt,0.0f });
            }
        }

    }

    while (const auto delta = wnd.mouse.ReadRawDelta())
    {
        if (!wnd.CursorEnabled())
        {
            camera.Rotate((float)delta->x, (float)delta->y);
        }
    }




    //if (ImGui::Begin("Simulation Speed"))
    //{
    //    ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 6.0f, "%.4f", 3.2f);
    //    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //    ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
    //}
    //ImGui::End();
    camera.ShowControlWND();

    light.CreateControlWindow();

    //wall.ShowWindow(wnd.Gfx(), "Wall");
    //goblin.ShowWindow(wnd.Gfx(), "GOBLIN_MODE");
    //nano.ShowWindow(wnd.Gfx(), "SUIT");
    //plane.SpawnWindow(wnd.Gfx());

    /*nano.ShowWindow();
    nano2.ShowWindow();
    plane.SpawnWindow(wnd.Gfx());*/
   // cube.SpawnWindow(wnd.Gfx());

    sponza.ShowWindow(wnd.Gfx(), "Sponza");

    bluePlane.SpawnWindow(wnd.Gfx(), "Blue Plane");
    redPlane.SpawnWindow(wnd.Gfx(), "Red Plane");
    yellowPlane.SpawnWindow(wnd.Gfx(), "Yellow Plane");



    //ShowRawInputWindow();

    wnd.Gfx().EndFrame();
}