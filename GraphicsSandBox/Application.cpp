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

GDIPManager gdipm;

using namespace DVS;

Application::Application() : wnd( 1280, 720, "Window" ), light(wnd.Gfx())
{

    wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f,  0.5f, 40.0f));
   // wnd.DisableCursor();
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

    nano.Draw(wnd.Gfx());


    //const DirectX::XMFLOAT3 material = { 1.0f, 1.0f, 1.0f };
    //auto model = std::make_unique<TestLoadedModel>(wnd.Gfx(), material, 1.5);
    //model->Draw(wnd.Gfx());

    light.Draw(wnd.Gfx());

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
    nano.ShowWindow();

    //ShowRawInputWindow();

    wnd.Gfx().EndFrame();
}