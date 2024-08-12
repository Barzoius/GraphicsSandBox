#include "Graphics.h"
#include "GFX_MACROS.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__,__FILE__,hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__,__FILE__, (hr))

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif


Graphics::Graphics(HWND hWnd) 
{

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1; /// Double Buffering
    sd.OutputWindow = hWnd;  /*(HWND)69696969;*/
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    UINT creationFlags = 0u;
#if defined(_DEBUG)
    // If the project is in a debug build, enable the debug layer.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr;

    GFX_THROW_INFO( D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwapChain,
        &pDevice,
        nullptr, /// D3D_FEATURE_LEVEL
        &pContext ) );


    Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;

    GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));

    GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
}


//Graphics::~Graphics()
//{
//    //if (pTarget != nullptr) pTarget->Release();
//
//    //if (pContext != nullptr) pContext->Release();
//
//    //if (pSwapChain != nullptr) pSwapChain->Release();
//
//    //if (pDevice != nullptr) pDevice->Release();
//}

void Graphics::EndFrame()
{
   
    HRESULT hr;

#ifndef NDEBUG
    infoManager.Set();
#endif

    if (FAILED(hr = pSwapChain -> Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else
        {
           throw GFX_EXCEPT(hr);
        }
    }
   
}


Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> INFOMessages) noexcept
    :
    Exception(line, file),
    hr(hr)
{

    for (const auto& m : INFOMessages)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }

}

const char* Graphics::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[ERROR CODE] " << GetErrorCode() << std::endl
        << "[ERROR STRING] " << GetErrorString() << std::endl
        << "[DESCRIPTION] " << GetErrorString() << std::endl;

    if (!info.empty())
        oss << "\n[ERROR INFO]\n" << GetErrorInfo() << std::endl << std::endl;
    
       oss << GetOriginString();

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
    return "[GRAPHICS_EXCEPTION]";
}

std::string Graphics::HrException::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf),
        0, nullptr);

    if (nMsgLen == 0) return "[UNIDENTIFIED_ERROR_CODE]";

    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
    return TranslateErrorCode(hr);
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
    return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
    return info;
}


Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file)
{
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }

    if (!info.empty())
    {
        info.pop_back();
    }
}


const char* Graphics::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[ERROR INFO]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
    return "GRAHICS INFO EXCEPTION";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
    return info;
}

void Graphics::DrawTrig()
{
    namespace WRL = Microsoft::WRL;

    HRESULT hr;

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

    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;

    pContext->IASetVertexBuffers(0u, 1u,
        pVertexBuffer.GetAddressOf(), &stride, &offset);

    WRL::ComPtr<ID3DBlob> pBlob;

    ///PIXEL

    WRL::ComPtr<ID3D11PixelShader> pPixelShader;

    GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));

    GFX_THROW_INFO(pDevice->CreatePixelShader(
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        nullptr, &pPixelShader));

    pContext->PSSetShader(pPixelShader.Get(), 0, 0);


    ///VERTEX

    WRL::ComPtr<ID3D11VertexShader> pVertexShader;
    
    GFX_THROW_INFO( D3DReadFileToBlob(L"VertexShader.cso", &pBlob));

    GFX_THROW_INFO(pDevice->CreateVertexShader(
        pBlob -> GetBufferPointer(),
        pBlob -> GetBufferSize(), 
        nullptr, &pVertexShader));

    pContext->VSSetShader(pVertexShader.Get(), 0, 0);



    ///VERTEX LAYOUT

    WRL::ComPtr<ID3D11InputLayout> pInputLayout;

    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,0, 0,
                        D3D11_INPUT_PER_VERTEX_DATA, 0 },

    };


    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied, 
        (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ) );

    pContext->IASetInputLayout(pInputLayout.Get());

    ///TARGET

    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);


    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D11_VIEWPORT vp;
    vp.Width = 800;
    vp.Height = 600;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    pContext->RSSetViewports(1u, &vp);

    GFX_THROW_INFO_ONLY(pContext->Draw((UINT)std::size(vertices), 0u));
}