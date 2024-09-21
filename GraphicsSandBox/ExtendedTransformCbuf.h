#pragma once


#include "TransformCbuf.h"


namespace Bind
{
    class ExtendedTransformCbuf : public TransformCbuf
    {
    public:
        ExtendedTransformCbuf(Graphics& gfx, const Drawable& parent, UINT vertexSlot = 0u, UINT pixelSlot = 0u);

        void Bind(Graphics& gfx) noexcept override;

    protected:
        void Update_Bind(Graphics& gfx, const Transforms& trans) noexcept;

    private:
        static std::unique_ptr<PixelConstantBuffer<Transforms>> pPcbuf;
    };
}