#include "Sheet.h"
#include "BindableObjects.h"
#include "Plane.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"


#include "GFX_MACROS.h"


Sheet::Sheet(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			struct
			{
				float u;
				float v;
			} tex;
		};
		
		auto model = Plane::Make<Vertex>();
		model.vertices[0].tex = { 0.0f,0.0f };
		model.vertices[1].tex = { 1.0f,0.0f };
		model.vertices[2].tex = { 0.0f,1.0f };
		model.vertices[3].tex = { 1.0f,1.0f };

		AddStaticBind(std::make_unique<Bind::Texture>(gfx, Surface::FromFile("Resources\\Images\\RobloxChadFace.png")));

		AddStaticBind(std::make_unique<Bind::VertexBuffer>(gfx, model.vertices));

		AddStaticBind(std::make_unique<Bind::Sampler>(gfx));

		auto pvs = std::make_unique<Bind::VertexShader>(gfx, L"TexVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<Bind::PixelShader>(gfx, L"TexPS.cso"));

		AddStaticIndexBuffer(std::make_unique<Bind::IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<Bind::InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<Bind::TransformCbuf>(gfx, *this));
}

void Sheet::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}

Sheet::Sheet(Graphics& gfx)
{
	namespace dx = DirectX;

		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};

		auto model = Plane::Make<Vertex>();

		AddStaticBind(std::make_unique<Bind::VertexBuffer>(gfx, model.vertices));


		auto pvs = std::make_unique<Bind::VertexShader>(gfx, L"VertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<Bind::PixelShader>(gfx, L"PixelShader.cso"));

		struct PixelShaderConstants
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};
		const PixelShaderConstants cb2 =
		{
			{
				{ 0.18f,0.28f,0.42f },
				{ 0.682f,0.729f,0.765f },
				{ 0.43f,0.15f,0.5f },
				{ 0.42f,0.10f,0.29f },
				{ 0.31f,0.41f,0.10f },
				{ 0.09f,0.24f,1.38f },
				{ 0.98f,0.41f,0.0f },
				{ 0.635f,1.0f,0.145f },

			}
		};

		AddStaticBind(std::make_unique<Bind::PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));

		AddStaticIndexBuffer(std::make_unique<Bind::IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<Bind::InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	


	AddBind(std::make_unique<Bind::TransformCbuf>(gfx, *this));
}