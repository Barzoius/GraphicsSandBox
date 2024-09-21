#include "ExtendedTransformCbuf.h"

namespace Bind
{
    ExtendedTransformCbuf::ExtendedTransformCbuf(Graphics& gfx, const Drawable& parent, UINT vertexSlot, UINT pixelSlot)
        :
        TransformCbuf(gfx, parent, vertexSlot)
    {
        if (!pPcbuf)
        {
            pPcbuf = std::make_unique<PixelConstantBuffer<Transforms>>(gfx, pixelSlot);
        }

    }

    void Bind::ExtendedTransformCbuf::Bind(Graphics& gfx) noexcept
    {
        const auto trans = GetTransforms(gfx);
        TransformCbuf::Update_Bind(gfx, trans); //vertex
        Update_Bind(gfx, trans); // pixel
    }

    void ExtendedTransformCbuf::Update_Bind(Graphics& gfx, const Transforms& trans) noexcept
    {
        pPcbuf->Update(gfx, trans);
        pPcbuf->Bind(gfx);
    }



    std::unique_ptr<PixelConstantBuffer<TransformCbuf::Transforms>> ExtendedTransformCbuf::pPcbuf;
}