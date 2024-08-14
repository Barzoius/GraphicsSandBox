#pragma once

#include "Bindable.h"
#include "GFX_MACROS.h"

class VertexBuffer : public Bindable
{
public:
    template <typename VERTEX>
    VertexBuffer(Graphics& gfx, const std::vector<VERTEX>& vertices)
        :
        stride(sizeof(VERTEX))
    {
        INFO_MANAGER(gfx);

        D3D11_BUFFER_DESC BD = {};
        BD.ByteWidth = UINT(sizeof(VERTEX) * vertices.size());
        BD.Usage = D3D11_USAGE_DEFAULT;
        BD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        BD.CPUAccessFlags = 0u;
        BD.MiscFlags = 0u;
        BD.StructureByteStride = sizeof(VERTEX);

        D3D11_SUBRESOURCE_DATA SD = {};
        SD.pSysMem = vertices.data();
        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&BD, &SD, &pVertexBuffer));

    }

    void Bind(Graphics& gfx) noexcept override;

protected:
    UINT stride;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};