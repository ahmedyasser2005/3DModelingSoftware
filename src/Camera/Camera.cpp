#include "Camera.h"
#include <algorithm>
#include <cmath>

using namespace DirectX;

Camera::Camera() noexcept
    : m_Target(0.0f, 0.0f, 0.0f),
      m_Pitch(DirectX::XMConvertToRadians(20.0f)),
      m_Yaw(DirectX::XMConvertToRadians(45.0f)),
      m_Radius(5.0f),
      m_Fov(DirectX::XMConvertToRadians(45.0f)),
      m_NearZ(0.1f),
      m_FarZ(1000.0f),
      m_LastMouseX(0),
      m_LastMouseY(0),
      m_IsFirstFrame(true),
      m_IsDirty(true)
{
    XMMATRIX identity = XMMatrixIdentity();
    XMStoreFloat4x4(&m_ViewMatrix, identity);
    XMStoreFloat4x4(&m_ProjMatrix, identity);
    XMStoreFloat4x4(&m_ViewProjMatrix, identity);
}

void Camera::SetTarget(const DirectX::XMFLOAT3& target) noexcept
{
    m_Target  = target;
    m_IsDirty = true;
}

void Camera::SetRadius(float radius) noexcept
{
    m_Radius  = radius;
    m_IsDirty = true;
}

void Camera::Update(const InputHandler& input, float screenWidth, float screenHeight, float deltaTime) noexcept
{
    // Delta time is skipped here as manual delta
    // calculation handles input pooling smoothly
    (void)deltaTime;

    int32_t currentMouseX = input.GetMouseX();
    int32_t currentMouseY = input.GetMouseY();

    if (m_IsFirstFrame)
    {
        m_LastMouseX   = currentMouseX;
        m_LastMouseY   = currentMouseY;
        m_IsFirstFrame = false;
        return;
    }

    int32_t dx = currentMouseX - m_LastMouseX;
    int32_t dy = currentMouseY - m_LastMouseY;

    bool isOrbitActive = input.IsMouseButtonDown(MouseButton::Left) && input.IsKeyDown(KeyCode::Shift);
    bool isPanActive   = input.IsMouseButtonDown(MouseButton::Left) && !input.IsKeyDown(KeyCode::Shift);

    // Rotational Orbiting
    if (isOrbitActive && (dx != 0 || dy != 0))
    {
        constexpr float sensitivity = 0.005f;
        m_Yaw += static_cast<float>(dx) * sensitivity;
        m_Pitch += static_cast<float>(dy) * sensitivity;

        // Prevent gimbal lock by clamping vertical rotation
        constexpr float pitchLimit = DirectX::XMConvertToRadians(85.0f);
        m_Pitch                    = std::clamp(m_Pitch, -pitchLimit, pitchLimit);

        m_IsDirty = true;
    }
    // Linear Panning
    else if (isPanActive && (dx != 0 || dy != 0))
    {
        XMMATRIX view = XMLoadFloat4x4(&m_ViewMatrix);

        // Extract camera coordinate space right and up vectors directly from rows of the view matrix
        XMVECTOR localRight = XMVectorSet(view.r[0].m128_f32[0], view.r[1].m128_f32[0], view.r[2].m128_f32[0], 0.0f);
        XMVECTOR localUp    = XMVectorSet(view.r[0].m128_f32[1], view.r[1].m128_f32[1], view.r[2].m128_f32[1], 0.0f);

        // Scaling pan speed dynamically based on focal distance feels highly intuitive
        float speedFactor = m_Radius * 0.0015f;

        // Calculate world-space translation step vector
        // **Inverse screen movement maps naturally to drag**
        XMVECTOR translation =
            (localRight * (static_cast<float>(-dx) * speedFactor)) + (localUp * (static_cast<float>(dy) * speedFactor));

        m_Target.x += XMVectorGetX(translation);
        m_Target.y += XMVectorGetY(translation);
        m_Target.z += XMVectorGetZ(translation);

        m_IsDirty = true;
    }

    // Zoom Operations
    float mouseWheelDelta = input.GetMouseWheelDelta();
    if (mouseWheelDelta != 0.0f)
    {
        float zoomSpeed = m_Radius * 0.1f;
        m_Radius -= mouseWheelDelta * zoomSpeed;

        // Lower bounding limit prevents flipping orientation coordinates backward
        m_Radius = std::max(0.2f, m_Radius);

        m_IsDirty = true;
    }

    m_LastMouseX = currentMouseX;
    m_LastMouseY = currentMouseY;

    if (m_IsDirty || screenWidth > 0.0f)
    {
        RecalculateMatrices(screenWidth, screenHeight);
        m_IsDirty = false;
    }
}

void Camera::RecalculateMatrices(float screenWidth, float screenHeight) noexcept
{
    if (screenWidth <= 0.0f || screenHeight <= 0.0f)
        return;

    float cosPitch = std::cos(m_Pitch);
    float sinPitch = std::sin(m_Pitch);
    float cosYaw   = std::cos(m_Yaw);
    float sinYaw   = std::sin(m_Yaw);

    float posX = m_Target.x + m_Radius * cosPitch * sinYaw;
    float posY = m_Target.y + m_Radius * sinPitch;
    float posZ = m_Target.z + m_Radius * cosPitch * cosYaw;

    XMVECTOR eyePos    = XMVectorSet(posX, posY, posZ, 1.0f);
    XMVECTOR targetPos = XMLoadFloat3(&m_Target);
    XMVECTOR worldUp   = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtLH(eyePos, targetPos, worldUp);
    XMStoreFloat4x4(&m_ViewMatrix, view);

    float    aspectRatio = screenWidth / screenHeight;
    XMMATRIX proj        = XMMatrixPerspectiveFovLH(m_Fov, aspectRatio, m_NearZ, m_FarZ);
    XMStoreFloat4x4(&m_ProjMatrix, proj);

    XMStoreFloat4x4(&m_ViewProjMatrix, view * proj);
}
