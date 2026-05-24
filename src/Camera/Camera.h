#pragma once

#include "Platform/InputHandler.h"
#include <DirectXMath.h>
#include <cstdint>

class Camera final
{
  public:
    Camera() noexcept;
    ~Camera() noexcept = default;

    void Update(const InputHandler& input, float screenWidth, float screenHeight, float deltaTime) noexcept;

    DirectX::XMFLOAT4X4 GetViewMatrix() const noexcept
    {
        return m_ViewMatrix;
    }
    DirectX::XMFLOAT4X4 GetProjectionMatrix() const noexcept
    {
        return m_ProjMatrix;
    }
    DirectX::XMFLOAT4X4 GetViewProjMatrix() const noexcept
    {
        return m_ViewProjMatrix;
    }

    void SetTarget(const DirectX::XMFLOAT3& target) noexcept;
    void SetRadius(float radius) noexcept;

  private:
    void RecalculateMatrices(float screenWidth, float screenHeight) noexcept;

    DirectX::XMFLOAT3 m_Target;
    float             m_Pitch;  // Vertical rotation (radians)
    float             m_Yaw;    // Horizontal rotation (radians)
    float             m_Radius; // Distance from target focal point

    float m_Fov;
    float m_NearZ;
    float m_FarZ;

    DirectX::XMFLOAT4X4 m_ViewMatrix;
    DirectX::XMFLOAT4X4 m_ProjMatrix;
    DirectX::XMFLOAT4X4 m_ViewProjMatrix;

    // Input Tracking Cache
    int32_t m_LastMouseX;
    int32_t m_LastMouseY;
    bool    m_IsFirstFrame;
    bool    m_IsDirty;
};
