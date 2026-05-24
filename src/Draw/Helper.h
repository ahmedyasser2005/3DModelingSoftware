#pragma once


#include "CubicBezier.h"
#include <DirectXMath.h>
#include <Windows.h>
#include <WinUser.h>

namespace Draw
{
DirectX::XMFLOAT2 Project(const DirectX::XMFLOAT3& worldPos,
                          const DirectX::XMFLOAT4X4& viewProj,
                          float screenWidth,
                          float screenHeight)
{

    DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&worldPos);
    DirectX::XMMATRIX vp = DirectX::XMLoadFloat4x4(&viewProj);
    DirectX::XMVECTOR clipPos = DirectX::XMVector3TransformCoord(p, vp);

    DirectX::XMFLOAT3 clip;
    DirectX::XMStoreFloat3(&clip, clipPos);

    float screenX = (clip.x + 1.0f) * 0.5f * screenWidth;
    float screenY = (1.0f - clip.y) * 0.5f * screenHeight;

    return DirectX::XMFLOAT2(screenX, screenY);
}
} // namespace Draw