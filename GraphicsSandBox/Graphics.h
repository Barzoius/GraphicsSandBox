#pragma once


#include "ModException.h"
#include <d3d11.h>
#include <dxgidebug.h>
#include <wrl.h>
#include "ErrorInfoManager.h"

#include "sstream"



class Graphics
{
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
    Graphics( HWND  hwnd );
    Graphics(const Graphics&) = delete;
    Graphics& operator = (const Graphics&) = delete;

    ~Graphics() = default;

    void EndFrame();
    void ClearBuffer(float red, float green, float blue) noexcept;

    void DrawTrig()
    {
        namespace WRL = Microsoft::WRL;
        
        struct Vertex
        {
            float x, y;
        };

        const Vertex vertices[] =
        {
            {0.0f, 0.5f},
            {0.5f, -0.5f},
            {-0.5f, -0.5f}
        };

        WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

        D3D11_BUFFER_DESC bd = {};
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.CPUAccessFlags = 0u;
        bd.MiscFlags = 0u;
        bd.ByteWidth = sizeof(vertices);
        bd.StructureByteStride = sizeof(Vertex);
        
        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = vertices;

        pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

        const UINT stride = sizeof(Vertex);
        const UINT offset = 0u;

        pContext->IASetVertexBuffers(0u, 1u,
                                 &pVertexBuffer, &stride, &offset);

        pContext->Draw(3u, 0u);
    }
   

private:

#ifndef NDEBUG
    ErrorInfoManager infoManager;
#endif

    Microsoft::WRL::ComPtr <IDXGISwapChain> pSwapChain = nullptr;
    Microsoft::WRL::ComPtr <ID3D11Device> pDevice = nullptr;
    Microsoft::WRL::ComPtr <ID3D11DeviceContext> pContext = nullptr; /// ImmediateContext
    Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pTarget = nullptr;
};