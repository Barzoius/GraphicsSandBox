#pragma once

#include "Bindable.h"

namespace Bind
{
    class PixelShader : public Bindable
    {
    public:
        PixelShader(Graphics& gfx, const std::string& path) noexcept;
        void Bind(Graphics& gfx) noexcept override;

        static std::shared_ptr<PixelShader> Resolve(Graphics& gfx, const std::string& path);
        static std::string GenerateUID(const std::string& path);
        std::string GetUID() const noexcept override;

    protected:
        std::string pixelPath;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
    };

}