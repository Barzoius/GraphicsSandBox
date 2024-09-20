#include "SolidSphere.h"
#include "BindableObjects.h"
#include "GFX_MACROS.h"
#include "VertexSystem.h"
#include "Sphere.h"


SolidSphere::SolidSphere(Graphics& gfx, float radius)
{

    struct Vertex
    {
        DirectX::XMFLOAT3 pos;
    };

    auto model = Sphere::Make();
    model.Transform(DirectX::XMMatrixScaling(radius, radius, radius));

    AddBind(std::make_shared<Bind::VertexBuffer>(gfx, model.vertices));
    AddBind(std::make_shared<Bind::IndexBuffer>(gfx, model.indices));

    auto pvs = std::make_shared<Bind::VertexShader>(gfx, "SolidVS.cso");
    auto pvsbc = pvs->GetBytecode();

    AddBind(std::move(pvs));

    AddBind(std::make_shared<Bind::PixelShader>(gfx, L"SolidPS.cso"));

    struct PSColorConstant
    {
        DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
        float padding;
    } colorConst;

    AddBind(std::make_shared<Bind::PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
    {
        { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
    };
    
    AddBind(std::make_shared<Bind::InputLayout>(gfx, ied, pvsbc));

    AddBind(std::make_shared<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    AddBind(std::make_shared<Bind::TransformCbuf>(gfx, *this));
}

void SolidSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
    this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
    return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}