#include "PixelShader.h"
#include "BindableCodex.h"

#include "GFX_MACROS.h"
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

namespace Bind
{
    PixelShader::PixelShader(Graphics& gfx, const std::string& path) noexcept
        :
        pixelPath(path)
    {

        INFO_MANAGER(gfx);

        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

        GFX_THROW_INFO(D3DReadFileToBlob(std::wstring{ path.begin(),path.end() }.c_str(), &pBlob));

        GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr, &pPixelShader));
    }

    void PixelShader::Bind(Graphics& gfx) noexcept
    {
        GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
    }

    std::shared_ptr<PixelShader> PixelShader::Resolve(Graphics& gfx, const std::string& path)
    {
        return Codex::Resolve<PixelShader>(gfx, path);
    }

    std::string PixelShader::GenerateUID(const std::string& path)
    {
        using namespace std::string_literals;
        return typeid(PixelShader).name() + "#"s + path;
    }

    std::string PixelShader::GetUID() const noexcept
    {
        return GenerateUID(pixelPath);
    }
}