#pragma once


#include "Bindable.h"


#include <array>
#include <optional>

namespace Bind
{
    class Blender : public Bindable
    {
    public:
        Blender(Graphics& gfx, bool blend, std::optional<float> factor = {});

        void Bind(Graphics& gfx) noexcept override;

        void SetFactor(float factor) noexcept;
        float GetFactor() const noexcept
            ;
        static std::shared_ptr<Blender> Resolve(Graphics& gfx, bool blending, std::optional<float> factor = {});
        static std::string GenerateUID(bool blending, std::optional<float> factor);
        std::string GetUID() const noexcept override;

    protected:
        bool blend;
        std::optional<std::array<float, 4>> mFactors;

        Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender; 
    };
}