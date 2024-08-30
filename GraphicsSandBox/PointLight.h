#pragma once


#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
    PointLight(Graphics& gfx, float radius = 0.5f);
    
    void CreateControlWindow() noexcept;
    void Reset() noexcept;
    void Draw(Graphics& gfx) const noexcept;
    void Bind(Graphics& gfx) const noexcept;

private:
    struct PointLightCBuf
    {
        alignas(16) DirectX::XMFLOAT3 pos;
        alignas(16) DirectX::XMFLOAT3 ambient;
        alignas(16) DirectX::XMFLOAT3 diffuseColor;
        float diffuseIntensity;
        float attenuationConst;
        float attenuationLin;
        float attenuationQuad;
    };

private:
    //DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
    PointLightCBuf cbData;
    mutable SolidSphere mesh;
    mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};