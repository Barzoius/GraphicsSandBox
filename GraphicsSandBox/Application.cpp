#include "Application.h"
#include "Cuboid.h"
#include "Math.h"
#include "Surface.h"
#include "GDIPManager.h"
#include "Torus.h"

#include "ImGui/imgui.h"


#include <sstream>
#include <memory>
#include <algorithm>

#include "Ball.h"
#include "Doughnut.h"
#include "Sheet.h"
#include "Pipe.h"

GDIPManager gdipm;

Application::Application() : wnd( 800, 600, "Window" ), light(wnd.Gfx())
{
    class Factory
    {
    public:
        Factory(Graphics& gfx)
            :
            gfx(gfx)
        {}
        std::unique_ptr<Drawable> operator()()
        {
         
            const DirectX::XMFLOAT3 material = { cdist(rng), cdist(rng), cdist(rng) };
            return std::make_unique<Cuboid>(
                gfx, rng, adist, ddist,
                odist, rdist, bdist, material
            );
        }
    private:
        Graphics& gfx;
        std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> adist{ 0.0f,FPI * 2.0f };
        std::uniform_real_distribution<float> ddist{ 0.0f,FPI * 0.5f };
        std::uniform_real_distribution<float> odist{ 0.0f,FPI * 0.08f };
        std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
        std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
        std::uniform_real_distribution<float> cdist{0.0f, 1.0f};
    };

    drawables.reserve(nDrawables);
    std::generate_n(std::back_inserter(drawables), nDrawables, Factory{ wnd.Gfx() });

    wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
   
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

        if (wnd.kbd.KeyIsPressed(VK_MENU))
        {
            MessageBox(nullptr, "Smth", "SPACE KEY WAS PRESSED",
                MB_OK | MB_ICONEXCLAMATION);
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

    //wnd.Gfx().BeginFrame(0.91f, 0.64f, 0.09f ); //nice yellow
    wnd.Gfx().BeginFrame(0.0f, 0.0f, 0.109f);
    wnd.Gfx().SetCamera(camera.GetMatrix());

    light.Bind(wnd.Gfx(), camera.GetMatrix());

    for (auto& d : drawables)
    {
        d->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
        d->Draw(wnd.Gfx());
    }

    light.Draw(wnd.Gfx());



    if (ImGui::Begin("Simulation Speed"))
    {
        ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 6.0f, "%.4f", 3.2f);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
    }
    ImGui::End();
    camera.ShowControlWND();
    light.CreateControlWindow();

    wnd.Gfx().EndFrame();
}