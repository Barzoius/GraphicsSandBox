#include "Application.h"
#include "Cuboid.h"
#include <sstream>

Application::Application() : wnd( 800, 600, "Window" ) 
{
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
    std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
    std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
    std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
    for (auto i = 0; i < 80; i++)
    {
        cuboids.push_back(std::make_unique<Cuboid>(
            wnd.Gfx(), rng, adist,
            ddist, odist, rdist
        ));
    }

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
    //const float t = timer.Peek();
    //std::ostringstream oss;
    //oss << "TimePassed: " 
    //    << std::setprecision(1) 
    //    << std::fixed << t << " s" 
    //    << std::endl;
    //wnd.SetTitle(oss.str());
    ///*wnd.Gfx().ClearBuffer(c, c, );*/
    //wnd.Gfx().DrawTrig();

    auto dt = timer.Mark();
    wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
    for (auto& b : cuboids)
    {
        b->Update(dt);
        b->Draw(wnd.Gfx());
    }

    wnd.Gfx().EndFrame();
}