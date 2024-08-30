#pragma once

#include "Bindable.h"

#include "GFX_MACROS.h"

template <class C>
class ConstantBuffer : public Bindable
{
public:


    ConstantBuffer(Graphics& gfx, const C& consts, UINT slot = 0u)
        :
        slot(slot)
    {
        INFO_MANAGER(gfx);

        D3D11_BUFFER_DESC CBD;
        CBD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        CBD.Usage = D3D11_USAGE_DYNAMIC;
        CBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        CBD.MiscFlags = 0u;
        CBD.ByteWidth = sizeof(consts);
        CBD.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA CSD = {};
        CSD.pSysMem = &consts;
        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&CBD, &CSD, &pConstantBuffer));
    }

    ConstantBuffer(Graphics& gfx, UINT slot = 0u)
        :
        slot(slot)
    {
        INFO_MANAGER(gfx);

        D3D11_BUFFER_DESC CBD;
        CBD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        CBD.Usage = D3D11_USAGE_DYNAMIC;
        CBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        CBD.MiscFlags = 0u;
        CBD.ByteWidth = sizeof(C);
        CBD.StructureByteStride = 0u;
        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&CBD, nullptr, &pConstantBuffer));
    }

    void Update(Graphics& gfx, const C& consts)
    {
        INFO_MANAGER(gfx);

        D3D11_MAPPED_SUBRESOURCE msr;
        GFX_THROW_INFO(GetContext(gfx)->Map(
            pConstantBuffer.Get(), 0u,
            D3D11_MAP_WRITE_DISCARD, 0u, &msr));

        memcpy(msr.pData, &consts, sizeof(consts));
        GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
    }

protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
    UINT slot;
};

template <typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
    using ConstantBuffer<C>::pConstantBuffer;
    using ConstantBuffer<C>::slot;
    using Bindable::GetContext;

public:
    using ConstantBuffer<C>::ConstantBuffer;

    void Bind(Graphics& gfx) noexcept override
    {
        GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
    }

};

template<class C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
    using ConstantBuffer<C>::pConstantBuffer;
    using ConstantBuffer<C>::slot;
    using Bindable::GetContext;

public:
    using ConstantBuffer<C>::ConstantBuffer;
    void Bind(Graphics& gfx) noexcept override
    {
        GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
    }
};