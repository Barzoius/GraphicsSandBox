#include "Application.h"
#include <sstream>

Application::Application() : wnd( 800, 600, "Window" ) {}

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

void Application::DoFrame()
{
    const float t = timer.Peek();
    std::ostringstream oss;
    oss << "TimePassed: " 
        << std::setprecision(1) 
        << std::fixed << t << " s" 
        << std::endl;
    wnd.SetTitle(oss.str());
}