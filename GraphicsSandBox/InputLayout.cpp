#include "InputLayout.h"

#include "GFX_MACROS.h"

#include "BindableCodex.h"
#include "VertexSystem.h"

namespace Bind
{
    InputLayout::InputLayout(Graphics& gfx,
        DVS::VertexLayout layout,
        ID3DBlob* pVertexShaderBytecode)
        :
        mLayout(std::move(layout))
    {
        INFO_MANAGER(gfx);

        const auto D3DLayout = mLayout.GetD3DLayout();

        GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
            D3DLayout.data(),
            (UINT)D3DLayout.size(),
            pVertexShaderBytecode->GetBufferPointer(),
            pVertexShaderBytecode->GetBufferSize(),
            &pInputLayout));
    }

    void InputLayout::Bind(Graphics& gfx) noexcept
    {
        GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
    }

    std::shared_ptr<InputLayout> InputLayout::Resolve(Graphics& gfx,
        const DVS::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode)
    {
        return Codex::Resolve<InputLayout>(gfx, layout, pVertexShaderBytecode);
    }


    std::string InputLayout::GenerateUID(const DVS::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode)
    {
        using namespace std::string_literals;
        return typeid(InputLayout).name() + "#"s + layout.GetCode();
    }

    std::string InputLayout::GetUID() const noexcept
    {
        return GenerateUID(mLayout);
    }

}