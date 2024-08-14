#include "InputLayout.h"

#include "GFX_MACROS.h"

InputLayout::InputLayout(Graphics& gfx,
                        const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
                        ID3DBlob* pVertexShaderBytecode)
{
    INFO_MANAGER(gfx);

    GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
    layout.data(),
    (UINT)layout.size(),
    pVertexShaderBytecode->GetBufferPointer(),
    pVertexShaderBytecode->GetBufferSize(),
    &pInputLayout));
}

void InputLayout::Bind(Graphics& gfx)
{
    GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}