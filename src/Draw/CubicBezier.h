#pragma once

#include <DirectXMath.h>
#include <cstdint>

class Renderer;

namespace Draw
{
void CurveCubicBezier(Renderer& renderer,
                      const DirectX::XMFLOAT4X4& viewProj,
                      DirectX::XMFLOAT3 p0,
                      DirectX::XMFLOAT3 p1,
                      DirectX::XMFLOAT3 p2,
                      DirectX::XMFLOAT3 p3,
                      uint32_t color);
} // namespace Draw