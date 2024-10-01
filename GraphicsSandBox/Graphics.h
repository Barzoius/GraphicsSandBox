#pragma once


#include "ModException.h"
#include <d3d11.h>
#include <dxgidebug.h>
#include <wrl.h>
#include "ErrorInfoManager.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <memory>
#include <random>
#include "sstream"

namespace Bind
{
    class Bindable;
}

class Graphics
{
    friend Bind::Bindable;
public:
    class Exception : public ModException
    {
        using ModException::ModException;
    };

    class HrException : public Exception
    {
    public:
        HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;

        const char* what() const noexcept override;
        const char* GetType() const noexcept override;

        static std::string TranslateErrorCode(HRESULT hr) noexcept;

        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
        //std::string GetErrorDescription() const noexcept;
        std::string GetErrorInfo() const noexcept;

    private:
        HRESULT hr;
        std::string info;
    };

    class InfoException : public Exception
    {
    public:
        InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        std::string GetErrorInfo() const noexcept;
    private:
        std::string info;
    };

    class DeviceRemovedException : public HrException
    {
        using HrException::HrException;

    public:
        const char* GetType() const noexcept override;
        std::string reason;
    };

public:
    Graphics(HWND hWnd, int width, int height);
    Graphics(const Graphics&) = delete;
    Graphics& operator = (const Graphics&) = delete;

    ~Graphics() = default;

    void EndFrame();
    void BeginFrame(float R, float G, float B) noexcept;

    void DrawIndexed(UINT count) noexcept;
    void SetProjection(DirectX::FXMMATRIX proj) noexcept;
    DirectX::XMMATRIX GetProjection() const noexcept;

   
    void EnableImgui() noexcept;
    void DisableImgui() noexcept;
    bool IsImguiEnabled() const noexcept;

    void SetCamera(DirectX::FXMMATRIX camera) noexcept;
    DirectX::XMMATRIX GetCamera() const noexcept;

private:
    bool imguiEnabled = true;
    DirectX::XMMATRIX projection;
    DirectX::XMMATRIX camera;

#ifndef NDEBUG
    ErrorInfoManager infoManager;
#endif

    Microsoft::WRL::ComPtr <IDXGISwapChain> pSwapChain = nullptr;
    Microsoft::WRL::ComPtr <ID3D11Device> pDevice = nullptr;
    Microsoft::WRL::ComPtr <ID3D11DeviceContext> pContext = nullptr; /// ImmediateContext
    Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pTarget = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};