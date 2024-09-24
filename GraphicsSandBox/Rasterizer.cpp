#include "Rasterizer.h"
#include "BindableObjects.h"

#include "GFX_MACROS.h"


namespace Bind
{
    Rasterizer::Rasterizer(Graphics& gfx, bool bothSides)
        : 
        bothSides(bothSides)
    {
        INFO_MANAGER(gfx);

        D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});


        ///---DISABLE BACK FACE CULLING---///
        rasterDesc.CullMode = bothSides ? D3D11_CULL_NONE : D3D11_CULL_BACK;

        GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer));
	}

    void Rasterizer::Bind(Graphics& gfx) noexcept
    {
       GetContext(gfx)->RSSetState(pRasterizer.Get());
    }

    std::shared_ptr<Rasterizer> Rasterizer::Resolve(Graphics& gfx, bool bothSides)
    {
        return Codex::Resolve<Rasterizer>(gfx, bothSides);
    }

    std::string Rasterizer::GenerateUID(bool bothSides)
    {
        using namespace std::string_literals;
        return typeid(Rasterizer).name() + "#"s + (bothSides ? "2s" : "bfc");
    }

    std::string Rasterizer::GetUID() const noexcept
    {
        return GenerateUID(bothSides);
    }


}