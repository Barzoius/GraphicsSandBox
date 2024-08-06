
#include "Window.h"
#include<sstream>


int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    try {
        Window wnd(800, 400, "SAHUF");

        MSG msg;
        BOOL gResult;

        while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

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
                }
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
        else
        {
            return msg.wParam;
        }

    }
    catch (const ModException& e)
    {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "[STANDARD_EXCEPTION]", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(nullptr, "????", "[UNKNOWN_EXCEPTION]", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;

}