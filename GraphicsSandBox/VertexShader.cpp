#include "VertexShader.h"
#include "GFX_MACROS.h"
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
{
    INFO_MANAGER(gfx);

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

    GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));

    GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        nullptr, &pVertexShader));
}

void VertexShader::Bind(Graphics& gfx)
{
    GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
    return pBytecodeBlob.Get();
}