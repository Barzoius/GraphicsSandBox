#include "Window.h"

#include <sstream>
#include "resource.h"

#include "WND_MACROS.h"

#include "ImGui/imgui_impl_win32.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = { 0 };

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(hInst, 
        MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = static_cast<HICON>(LoadImage(hInst, 
        MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

    RegisterClassEx(&wc);

}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

Window::Window(int width, int height, const char* name) 
    :
    width(width),
    height(height)
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0 )
    {
        throw HWND_LAST_EXCEPT();
    };

    hWnd = CreateWindow(
      WindowClass::GetName(), name,
      WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
      CW_USEDEFAULT, CW_USEDEFAULT,
      wr.right - wr.left, wr.bottom - wr.top,
      nullptr, nullptr, 
      WindowClass::GetInstance(), this
  );

    if (hWnd == nullptr) throw HWND_LAST_EXCEPT();


    ShowWindow(hWnd, SW_SHOWDEFAULT);

    ImGui_ImplWin32_Init(hWnd);

    pGfx = std::make_unique<Graphics>(hWnd);


    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01; // mouse page
    rid.usUsage = 0x02; // mouse usage
    rid.dwFlags = 0;
    rid.hwndTarget = nullptr;
    if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
    {
        throw HWND_LAST_EXCEPT();
    }


}

void Window::SetTitle(const std::string& title)
{
    if (SetWindowText(hWnd, title.c_str()) == 0)
    {
        throw HWND_LAST_EXCEPT();
    }
}

void Window::EnableCursor()
{
    cursorEnabled = true;
    ShowCursor();
    EnableImGuiMouse();
    FreeCursor();
}


void Window::DisableCursor()
{
    cursorEnabled = false;
    HideCursor();
    DisableImGuiMouse();
    ConfineCursor();
}

bool Window::CursorEnabled() const noexcept
{
    return cursorEnabled;
}

std::optional<int> Window::ProccessMessages()
{
    MSG msg;

    ///PeekMessage doesnt block on WM_QUIT
    while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

Graphics& Window::Gfx()
{
    if (!pGfx)
    {
        throw HWND_NOGFX_EXCEPT();
    }
    return *pGfx;
}

Window::~Window()
{ 
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(hWnd);
}

void Window::HideCursor()
{
    while (::ShowCursor(FALSE) >= 0);
}

void Window::ShowCursor()
{
    while (::ShowCursor(TRUE) < 0);
}

void Window::ConfineCursor()
{
    RECT rect;
    GetClientRect(hWnd, &rect);
    MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
    ClipCursor(&rect);
}

void Window::FreeCursor()
{
    ClipCursor(nullptr);
}

void Window::EnableImGuiMouse()
{
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImGuiMouse()
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);

}


LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    const auto imio = ImGui::GetIO();

    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_KILLFOCUS:
        kbd.ClearState();
        break;

    case WM_ACTIVATE:

        if (!cursorEnabled)
        {
            if (wParam & WA_ACTIVE)
            {

                ConfineCursor();
                HideCursor();
            }
            else
            {

                FreeCursor();
                ShowCursor();
            }
        }
        break;


    case WM_KEYDOWN:

    case WM_SYSKEYDOWN:

        if (imio.WantCaptureKeyboard)
        {
            break;
        }
        if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
        {
            kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        if (imio.WantCaptureKeyboard)
        {
            break;
        }
        kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;

    case WM_CHAR:
        if (imio.WantCaptureKeyboard)
        {
            break;
        }
        kbd.OnChar(static_cast<unsigned char>(wParam));
        break;

    case WM_MOUSEMOVE:
    {
       

        const POINTS point = MAKEPOINTS(lParam);

        if (!cursorEnabled)
        {
            if (!mouse.IsInWindow())
            {
                SetCapture(hWnd);
                mouse.OnMouseEnter();
                HideCursor();
            }
            break;
        }

        if (imio.WantCaptureKeyboard)
        {
            break;
        }

        if (point.x >= 0 && point.x < width &&
            point.y >= 0 && point.y < height)
        {
            mouse.OnMouseMove(point.x, point.y);
            
            if (!mouse.IsInWindow())
            {
                ///keep recieving mouse signals
                ///even with the mouse outside the window
                SetCapture(hWnd);
                mouse.OnMouseEnter();
            }
        }
        else
        {
            ///checks if lbutton/rbutton are down 
            ///so we can keep dragging
            if (wParam & (MK_LBUTTON | MK_RBUTTON))
            {
                mouse.OnMouseMove(point.x, point.y);
            }
            else
            {
                ReleaseCapture();
                mouse.OnMouseLeave();
            }
        }

      
    }

    case WM_LBUTTONDOWN:
    {
        SetForegroundWindow(hWnd);

        if (!cursorEnabled)
        {

            ConfineCursor();
            HideCursor();
        }

        if (imio.WantCaptureKeyboard)
        {
            break;
        }

        const POINTS point = MAKEPOINTS(lParam);
        mouse.OnLeftPressed(point.x, point.y);
        break;
    }

    case WM_RBUTTONDOWN:
    {
        if (imio.WantCaptureKeyboard)
        {
            break;
        }

        const POINTS point = MAKEPOINTS(lParam);
        mouse.OnRightPressed(point.x, point.y);
        break;
    }

    case WM_LBUTTONUP:
    {
        if (imio.WantCaptureKeyboard)
        {
            break;
        }
        const POINTS point = MAKEPOINTS(lParam);
        mouse.OnLeftReleased(point.x, point.y);
        
        if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height)
        {
            ReleaseCapture();
            mouse.OnMouseLeave();
        }
        break;
    }

    case WM_RBUTTONUP:
    {
        if (imio.WantCaptureKeyboard)
        {
            break;
        }
        const POINTS point = MAKEPOINTS(lParam);
        mouse.OnRightReleased(point.x, point.y);
        
        if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height)
        {
            ReleaseCapture();
            mouse.OnMouseLeave();
        }
        break;
    }

    case WM_MOUSEWHEEL:
    {
        if (imio.WantCaptureKeyboard)
        {
            break;
        }
        const POINTS point = MAKEPOINTS(lParam);

        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse.OnWheelDelta(point.x, point.y, delta);


        break;
    }

    case WM_INPUT:
    {

        if (!mouse.RawEnabled())
        {
            break;
        }

        UINT size;

        if (GetRawInputData(
            reinterpret_cast<HRAWINPUT>(lParam),
            RID_INPUT,
            nullptr,
            &size,
            sizeof(RAWINPUTHEADER)) == -1)
        {

            break;
        }
        rawBuffer.resize(size);

        if (GetRawInputData(
            reinterpret_cast<HRAWINPUT>(lParam),
            RID_INPUT,
            rawBuffer.data(),
            &size,
            sizeof(RAWINPUTHEADER)) != size)
        {

            break;
        }

        auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
        if (ri.header.dwType == RIM_TYPEMOUSE &&
            (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
        {
            mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
        }
        break;
    }
    }


    return DefWindowProc(hWnd, msg, wParam, lParam);
}

/// Window Exception

Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
    :
    ModException(line, file),
    hr(hr)
{}

const char* Window::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[ERROR CODE] " << GetErrorCode() << std::endl
        << "[DESCRIPTION] " << GetErrorString() << std::endl
        << GetOriginString();

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
    return "[WINDOW_EXCEPTION]";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM     |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL , SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf),
        0, nullptr);

    if (nMsgLen == 0) return "[UNIDENTIFIED_ERROR_CODE]";

    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
    return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
    return TranslateErrorCode(hr);
}

const char* Window::NoGFXException::GetType() const noexcept
{
    return "WINDOW_EXCEPTION::NO_GRAPHICS";
}

