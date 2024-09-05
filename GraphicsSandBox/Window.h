#pragma once
#include "ModWin.h"
#include "ModException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

#include <optional>
#include <memory>


class Window 
{

public:
    class Exception : public ModException
    {
        using ModException::ModException;
    public:
        Exception(int line, const char* file, HRESULT hr) noexcept;

        const char* what() const noexcept override;

        virtual const char* GetType() const noexcept override;
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;

    private:
        HRESULT hr;
    };

    class NoGFXException : public Exception
    {
    public:
        using Exception::Exception;
        const char* GetType() const noexcept override;
    };
private:

    class WindowClass
    {
    public:
        static const char* GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;
    private:
        WindowClass() noexcept;
        ~WindowClass();

        WindowClass(const WindowClass&) = delete;
        WindowClass& operator = (const WindowClass&) = delete;

        static constexpr const char* wndClassName = "Engine Window";
        static WindowClass wndClass;
        HINSTANCE hInst;
    };

public:
    Window(int width, int height, const char* name);
    ~Window();
    
    Window(const Window&) = delete;
    Window& operator = (const Window&) = delete;

    void SetTitle(const std::string& title);
    void EnableCursor();
    void DisableCursor();

    bool CursorEnabled() const noexcept;

    static std::optional<int> ProccessMessages();

    Graphics& Gfx();

private:
    void HideCursor();
    void ShowCursor();

    void EnableImGuiMouse();
    void DisableImGuiMouse();

    void ConfineCursor();
    void FreeCursor();

    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
    Keyboard kbd;
    Mouse mouse;

private:
    int width;
    int height;
    HWND hWnd;

    bool cursorEnabled = true;
    std::vector<BYTE> rawBuffer;

    std::unique_ptr<Graphics> pGfx;

};
