#pragma once

#include "IndexBuffer.h"

#include "GFX_MACROS.h"

namespace Bind
{
    IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
        :
        count((UINT)indices.size())
    {
        INFO_MANAGER(gfx);

        D3D11_BUFFER_DESC IDB = {};
        IDB.ByteWidth = UINT(count * sizeof(unsigned short));
        IDB.Usage = D3D11_USAGE_DEFAULT;
        IDB.BindFlags = D3D11_BIND_INDEX_BUFFER;
        IDB.CPUAccessFlags = 0u;
        IDB.MiscFlags = 0u;
        IDB.StructureByteStride = sizeof(unsigned short);

        D3D11_SUBRESOURCE_DATA ISD = {};
        ISD.pSysMem = indices.data();

        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&IDB, &ISD, &pIndexBuffer));
    }

    void IndexBuffer::Bind(Graphics& gfx) noexcept
    {
        GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
    }

    UINT IndexBuffer::GetCount() const noexcept
    {
        return count;
    }
}