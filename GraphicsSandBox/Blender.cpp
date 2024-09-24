#include "Blender.h"
#include "BindableCodex.h"

#include "GFX_MACROS.h"

namespace Bind
{
    Blender::Blender(Graphics& gfx, bool blending, std::optional<float> factors)
        :
        blend(blending)
    {
        INFO_MANAGER(gfx);

        if (factors)
        {
            mFactors.emplace();
            mFactors->fill(*factors);
        }

        D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
        auto& brt = blendDesc.RenderTarget[0];
        if (blend)
        {
            brt.BlendEnable = TRUE;

            if (factors)
            {
                brt.SrcBlend = D3D11_BLEND_BLEND_FACTOR;
                brt.DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
            }
            else
            {
                brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
                brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            }
        }
        
        GFX_THROW_INFO(GetDevice(gfx)->CreateBlendState(&blendDesc, &pBlender));
    }

    void Blender::Bind(Graphics& gfx) noexcept
    {
        const float* data = mFactors ? mFactors->data() : nullptr;
        GetContext(gfx)->OMSetBlendState(pBlender.Get(), data, 0xFFFFFFFFu);
    }

    void Blender::SetFactor(float factor) noexcept
    {
        assert(mFactors);
        return mFactors->fill(factor);
    }

    float Blender::GetFactor() const noexcept
    {
        assert(mFactors);
        return mFactors->front();
    }


    std::shared_ptr<Blender> Blender::Resolve(Graphics& gfx, bool blending, std::optional<float> factor)
    {
        return Codex::Resolve<Blender>(gfx, blending, factor); 
    }
    
    std::string Blender::GenerateUID(bool blending, std::optional<float> factor)
    {
        using namespace std::string_literals; 
        return typeid(Blender).name() + "#"s + (blending ? "b"s : "n"s) + (factor ? "#f"s + std::to_string(*factor) : "");
    }
            
    std::string Blender::GetUID() const noexcept
    {
        return GenerateUID(blend, mFactors ? mFactors->front() : std::optional<float>{});
    }
}
