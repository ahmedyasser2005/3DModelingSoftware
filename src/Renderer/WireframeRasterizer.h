#pragma once
#include <DirectXMath.h>
#include <cstdint>

class Renderer;
struct Mesh;

namespace WireframeRasterizer
{

bool ProjectToScreen(const DirectX::XMVECTOR& worldPos,
                     const DirectX::XMMATRIX& viewProj,
                     uint32_t                 screenWidth,
                     uint32_t                 screenHeight,
                     uint32_t*                outX,
                     uint32_t*                outY);

void Draw(Renderer&                  renderer,
          const Mesh&                mesh,
          const DirectX::XMFLOAT4X4& globalTransform,
          const DirectX::XMFLOAT4X4& viewProjMatrix);

} // namespace WireframeRasterizer