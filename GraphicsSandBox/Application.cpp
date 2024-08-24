#include "Application.h"
#include "Cuboid.h"
#include "Math.h"
#include "Surface.h"
#include "GDIPManager.h"
#include "Sheet.h"
#include "Torus.h"

#include "ImGui/imgui.h"


#include <sstream>
#include <memory>
#include <algorithm>

#include "Ball.h"
#include "Doughnut.h"

GDIPManager gdipm;

Application::Application() : wnd( 800, 600, "Window" ) 
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
            switch (typedist(rng))
            {
            case 0:
                return std::make_unique<Ball>(
                    gfx, rng, adist, ddist,
                    odist, rdist, latdist, longdist
                );
            case 1:

                return std::make_unique<Cuboid>(
                    gfx, rng, adist,
                    ddist, odist, rdist
                );
            case 2:
                //return std::make_unique<Melon>(
                //    gfx, rng, adist, ddist,
                //    odist, rdist, longdist, latdist
                //);
            case 3:
                return std::make_unique<Sheet>(gfx, rng, adist, ddist, odist, rdist);
            default:
                assert(false && "bad drawable type in factory");
                return {};
            }
        }
    private:
        Graphics& gfx;
        std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> adist{ 0.0f,FPI * 2.0f };
        std::uniform_real_distribution<float> ddist{ 0.0f,FPI * 0.5f };
        std::uniform_real_distribution<float> odist{ 0.0f,FPI * 0.08f };
        std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
        std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
        std::uniform_int_distribution<int> latdist{ 5,20 };
        std::uniform_int_distribution<int> longdist{ 10,40 };
        std::uniform_int_distribution<int> typedist{ 0,3 };
    };

    Factory f(wnd.Gfx());
    drawables.reserve(nDrawables);
    std::generate_n(std::back_inserter(drawables), nDrawables, f);


    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
    std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
    std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
    std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

    for (auto i = 0; i < 1; i++)
    {
        cuboids.push_back(std::make_unique<Cuboid>(
            wnd.Gfx(), rng, adist,
            ddist, odist, rdist
        ));
    }




    const auto s = Surface::FromFile("Resources\\Images\\RobloxChadFace.png");
    //Surface::CreateHeightMap(400, 400);

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

    wnd.Gfx().BeginFrame(0.91f, 0.64f, 0.09f );
    wnd.Gfx().SetCamera(camera.GetMatrix());

 //   for (auto& b : cuboids)
 //{
 //    b->Update(dt);
 //    b->Draw(wnd.Gfx());
 //}


    //Ball B(wnd.Gfx());
    //B.Draw(wnd.Gfx());

    Sheet P(wnd.Gfx());
    P.Draw(wnd.Gfx());

    //Cuboid C(wnd.Gfx());
    //C.Draw(wnd.Gfx());

    //Doughnut D(wnd.Gfx());
    //D.Draw(wnd.Gfx());

    //for (auto& d : drawables)
    //{
    //    //d->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
    //    d->Draw(wnd.Gfx());
    //}



    static char buffer[1024];
    if (ImGui::Begin("Simulation Stats"))
    {
        ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 4.0f);
        ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::InputText("...", buffer, sizeof(buffer));
    }
    ImGui::End();
    camera.ShowControlWND();

    wnd.Gfx().EndFrame();
}