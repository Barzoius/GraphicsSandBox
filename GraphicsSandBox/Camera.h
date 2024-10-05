#pragma once

#include "Graphics.h"
#include "Math.h"

class Camera
{
public:
    Camera();

    void ShowControlWND() noexcept;
    void Reset() noexcept;

    void Translate(DirectX::XMFLOAT3 translate) noexcept;
    void Rotate(float dx, float dy) noexcept;


    DirectX::XMFLOAT3 GetPos() const noexcept;

    DirectX::XMMATRIX GetMatrix() const noexcept;

private:
    DirectX::XMFLOAT3 pos;
    float pitch;
    float yaw;

    static constexpr float travelSpeed = 50.0f;
    static constexpr float rotationSpeed = 0.004f;
};