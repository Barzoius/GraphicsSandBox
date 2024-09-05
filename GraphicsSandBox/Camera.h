#pragma once

#include "Graphics.h"
#include "Math.h"

class Camera
{
public:
    Camera();

    DirectX::XMMATRIX GetMatrix() const noexcept;
    void ShowControlWND() noexcept;
    void Reset() noexcept;

    void Translate(DirectX::XMFLOAT3 translate) noexcept;
    void Rotate(float dx, float dy) noexcept;

private:
    DirectX::XMFLOAT3 pos;
    float pitch;
    float yaw;

    static constexpr float travelSpeed = 12.0f;
    static constexpr float rotationSpeed = 0.004f;
};