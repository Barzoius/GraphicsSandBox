#pragma once

#include "Bindable.h"
#include "GFX_MACROS.h"
#include "VertexSystem.h"

namespace Bind
{
    class VertexBuffer : public Bindable
    {
    public:
      
        VertexBuffer(Graphics& gfx, const DVS::VertexBuffer& vbuf)
            :
            stride((UINT)vbuf.GetLayout().Size())
        {
            INFO_MANAGER(gfx);

            D3D11_BUFFER_DESC BD = {};
            BD.ByteWidth = UINT(vbuf.SizeBytes());
            BD.Usage = D3D11_USAGE_DEFAULT;
            BD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            BD.CPUAccessFlags = 0u;
            BD.MiscFlags = 0u;
            BD.StructureByteStride = stride;

            D3D11_SUBRESOURCE_DATA SD = {};
            SD.pSysMem = vbuf.GetData();
            GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&BD, &SD, &pVertexBuffer));
        }

        void Bind(Graphics& gfx) noexcept override;

    protected:
        UINT stride;
        Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    };

}