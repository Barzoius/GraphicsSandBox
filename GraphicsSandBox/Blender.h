#pragma once


#include "Bindable.h"


#include <array>

namespace Bind
{
    class Blender : public Bindable
    {
    public:
        Blender(Graphics& gfx, bool blend);

        void Bind(Graphics& gfx) noexcept override;

        static std::shared_ptr<Blender> Resolve(Graphics& gfx, bool blending);
        static std::string GenerateUID(bool blending);
        std::string GetUID() const noexcept override;

    protected:
        bool blend;

        Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender; 
    };
}