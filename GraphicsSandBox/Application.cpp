#include "Application.h"
#include "Cuboid.h"
#include "Math.h"
#include "Surface.h"
#include "GDIPManager.h"
#include "Torus.h"
#include "TestLoadedModel.h"

#include "ImGui/imgui.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <sstream>
#include <memory>
#include <algorithm>

#include "Ball.h"
#include "Doughnut.h"
#include "Sheet.h"
#include "Pipe.h"

#include "VertexSystem.h"

GDIPManager gdipm;

using namespace DVS;

Application::Application() : wnd( 800, 600, "Window" ), light(wnd.Gfx())
{

    wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
    wnd.DisableCursor();
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
    MSG msg;
    BOOL gResult;

    while (true)
    {

      if(const auto enter_code = Window::ProccessMessages())
      {
          return *enter_code;
      }

      /// DO FRAME ///
      DoFrame();
      
        static int i = 0;
        while (!wnd.mouse.IsEmpty())
        {
            const auto e = wnd.mouse.Read();
            switch (e.GetType())
            {
            case Mouse::Event::Type::Leave:
                wnd.SetTitle("Outisde the window");
                break;

            case Mouse::Event::Type::Move:
            {
                std::ostringstream oss;
                oss << "Mouse Pos: (" << e.GetPosX() << ", "
                    << e.GetPosY() << ")" << std::endl;

                wnd.SetTitle(oss.str());
                break;
            }

            case Mouse::Event::Type::WheelUp:
                i++;
                {
                    std::ostringstream oss;
                    oss << "Up: " << i;
                    wnd.SetTitle(oss.str());
                }
                break;
            case Mouse::Event::Type::WheelDown:
                i--;
                {
                    std::ostringstream oss;
                    oss << "Down: " << i;
                    wnd.SetTitle(oss.str());
                }
                break;

            }

        }
    }

    if (gResult == -1)
    {
        return -1;
    }

    return msg.wParam;

}

Application::~Application()
{}


void Application::DoFrame()
{
    const auto dt = timer.Mark() * speedFactor;

    wnd.Gfx().BeginFrame(0.91f, 0.64f, 0.09f ); //nice yellow
    //wnd.Gfx().BeginFrame(0.0f, 0.0f, 0.109f); //dark 
    wnd.Gfx().SetCamera(camera.GetMatrix());

    light.Bind(wnd.Gfx(), camera.GetMatrix());

    const auto transform = DirectX::XMMatrixRotationRollPitchYaw(pos.roll, pos.pitch, pos.yaw) *
        DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
    nano.Draw(wnd.Gfx());


    //const DirectX::XMFLOAT3 material = { 1.0f, 1.0f, 1.0f };
    //auto model = std::make_unique<TestLoadedModel>(wnd.Gfx(), material, 1.5);
    //model->Draw(wnd.Gfx());

    light.Draw(wnd.Gfx());

    while (const auto e = wnd.kbd.ReadKey())
    {
        if (e->IsPressed() && e->GetCode() == VK_RETURN)
        {
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

        while (const auto delta = wnd.mouse.ReadRawDelta())
        {
            if (!wnd.CursorEnabled())
            {
                camera.Rotate(delta->x, delta->y);
            }
        }
    }




    if (ImGui::Begin("Simulation Speed"))
    {
        ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 6.0f, "%.4f", 3.2f);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
    }
    ImGui::End();
    camera.ShowControlWND();
    light.CreateControlWindow();
    nano.ShowWindow();

    ShowRawInputWindow();

    wnd.Gfx().EndFrame();
}