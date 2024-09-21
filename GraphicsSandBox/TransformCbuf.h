#pragma once
#include "ConstantBuffers.h"

#include "Drawable.h"
#include <DirectXMath.h>

namespace Bind
{
    class TransformCbuf : public Bindable
    {
    protected:
        struct Transforms
        {
            DirectX::XMMATRIX modelView;
            DirectX::XMMATRIX modelViewProj;
        };

    public:
        TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot = 0u);
        void Bind(Graphics& gfx) noexcept override;

    protected:
        void Update_Bind(Graphics& gfx, const Transforms& trans) noexcept;
        Transforms GetTransforms(Graphics&) noexcept;

    private:
        static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
        const Drawable& parent;
    };
}