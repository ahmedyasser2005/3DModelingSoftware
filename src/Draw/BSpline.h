#pragma once
#include "Renderer/Renderer.h"
#include <DirectXMath.h>
#include <vector>

namespace Draw
{
void CurveBSpline(Renderer& renderer,
                  const std::vector<DirectX::XMFLOAT3>& points,
                  uint32_t color,
                  int segmentsPerCurve = 32);
}