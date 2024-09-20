#pragma once

#include "Bindable.h"
#include "VertexSystem.h"

namespace Bind
{
    class InputLayout : public Bindable
    {
    public:
        InputLayout(Graphics& gfx,
            DVS::VertexLayout layout,
            ID3DBlob* pVertexShaderByteCode);

        void Bind(Graphics& gfx) noexcept override;

        static std::shared_ptr<InputLayout> Resolve(Graphics& gfx, const DVS::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode);
        static std::string GenerateUID(const DVS::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode = nullptr);
        std::string GetUID() const noexcept override;

    protected:
        DVS::VertexLayout mLayout;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

    };
}