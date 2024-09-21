#include "TransformCbuf.h"

namespace Bind
{
    TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
        :
        parent(parent)
    {
        if (!pVcbuf)
        {
            pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
        }
    }

    void TransformCbuf::Bind(Graphics& gfx) noexcept
    {
        Update_Bind(gfx, GetTransforms(gfx));
    }

    void TransformCbuf::Update_Bind(Graphics& gfx, const Transforms& trans) noexcept
    {
        pVcbuf->Update(gfx, trans);
        pVcbuf->Bind(gfx);
    }


    TransformCbuf::Transforms TransformCbuf::GetTransforms(Graphics& gfx) noexcept
    {

        const auto modelView = parent.GetTransformXM() * gfx.GetCamera();

        return {
            DirectX::XMMatrixTranspose(modelView),
            DirectX::XMMatrixTranspose(
                modelView *
                gfx.GetProjection())
        };
    }

    std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;

}