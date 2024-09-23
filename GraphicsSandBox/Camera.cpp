#include "Camera.h"
#include "ImGui/imgui.h"

Camera::Camera()
{
    Reset();
}

namespace DX = DirectX;

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
    //const auto pos = DirectX::XMVector3Transform(
    //    DirectX::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
    //    DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
    //);
    //return DirectX::XMMatrixLookAtLH(
    //    pos, DirectX::XMVectorZero(),
    //    DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    //) * DirectX::XMMatrixRotationRollPitchYaw(
    //    pitch, -yaw, roll
    //);

    using namespace DX;

    const DX::XMVECTOR fowardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    const auto lookVector = XMVector3Transform(fowardBaseVector,
        XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f));

    const auto camPos = XMLoadFloat3(&pos);
    const auto camTarget = camPos + lookVector;

    return XMMatrixLookAtLH(camPos, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));


    //return DirectX::XMMatrixTranslation(-pos.x, -pos.y, -pos.z) *
    //    DirectX::XMMatrixRotationRollPitchYaw(-pitch, -yaw, 0.0f);
}

void Camera::ShowControlWND() noexcept
{
    if (ImGui::Begin("Camera Controls"))
    {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
        ImGui::SliderAngle("Y", &pos.y, -80.0f, 80.0f , "%.1f");
        ImGui::SliderAngle("Z", &pos.z, -80.0f, 80.0f, "%.1f");
        ImGui::Text("Orientation");
        ImGui::SliderAngle("Pitch", &pitch, 0.9995f * -90.0f, 0.9995f * 90.0f);
        ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
        
        if (ImGui::Button("Reset"))
        {
            Reset();
        }

    }

    ImGui::End();
}


void Camera::Reset() noexcept
{
    pos = { -13.5f,6.0f,3.5f };
    pitch = 0.0f;
    yaw = 0.45f;

}


void Camera::Rotate(float dx, float dy) noexcept
{
    yaw = wrap_angle(yaw + dx * rotationSpeed);
    pitch = std::clamp(pitch + dy * rotationSpeed, 0.9995f * -FPI / 2.0f, 0.9995f * FPI / 2.0f);
}

void Camera::Translate(DirectX::XMFLOAT3 translation) noexcept
{
    DirectX::XMStoreFloat3(&translation, 
        DirectX::XMVector3Transform(
            DirectX::XMLoadFloat3(&translation),
            DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) * 
            DirectX::XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)));


    pos = { pos.x + translation.x,
            pos.y + translation.y,
            pos.z + translation.z
          };

}