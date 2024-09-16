#include "Texture.h"
#include "Surface.h"

#include "GFX_MACROS.h"

namespace Bind
{
    Texture::Texture(Graphics& gfx, const class Surface& S, unsigned int slot)
        :
        slot(slot)
    {
        INFO_MANAGER(gfx);

        D3D11_TEXTURE2D_DESC texDesc = {};
        texDesc.Width = S.GetWidth();
        texDesc.Height = S.GetHeight();
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = 0;
        texDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA SD = {};
        SD.pSysMem = S.GetBufferPtr();
        SD.SysMemPitch = S.GetWidth() * sizeof(Surface::Color);

        Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

        GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&texDesc, &SD, &pTexture));

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = texDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView));
    }


    void Texture::Bind(Graphics& gfx) noexcept
    {
        GetContext(gfx)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());
    }
}