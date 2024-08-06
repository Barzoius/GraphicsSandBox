
#include "Window.h"
#include "Application.h"
#include<sstream>


int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    try {
        return Application{}.Run();
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