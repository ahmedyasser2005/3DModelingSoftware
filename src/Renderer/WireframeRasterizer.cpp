#include "WireframeRasterizer.h"
#include "Draw/Line.h"
#include "Renderer/Mesh.h"
#include "Renderer/Renderer.h"

bool WireframeRasterizer::ProjectToScreen(const DirectX::XMVECTOR& worldPos,
                                          const DirectX::XMMATRIX& viewProj,
                                          uint32_t                 screenWidth,
                                          uint32_t                 screenHeight,
                                          uint32_t*                outX,
                                          uint32_t*                outY)
{
    DirectX::XMVECTOR clipPos = DirectX::XMVector3TransformCoord(worldPos, viewProj);
    DirectX::XMFLOAT4 clip;
    DirectX::XMStoreFloat4(&clip, clipPos);

    if (clip.z < 0.0f || clip.z > 1.0f || clip.x < -1.0f || clip.x > 1.0f || clip.y < -1.0f || clip.y > 1.0f)
    {
        return false;
    }

    *outX = static_cast<uint32_t>((clip.x + 1.0f) * 0.5f * screenWidth);
    *outY = static_cast<uint32_t>((1.0f - clip.y) * 0.5f * screenHeight);
    return true;
}

void WireframeRasterizer::Draw(Renderer&                  renderer,
                               const Mesh&                mesh,
                               const DirectX::XMFLOAT4X4& globalTransform,
                               const DirectX::XMFLOAT4X4& viewProjMatrix)
{
    DirectX::XMMATRIX model         = DirectX::XMLoadFloat4x4(&globalTransform);
    DirectX::XMMATRIX viewProj      = DirectX::XMLoadFloat4x4(&viewProjMatrix);
    DirectX::XMMATRIX modelViewProj = model * viewProj;

    uint32_t screenWidth  = renderer.GetWidth();
    uint32_t screenHeight = renderer.GetHeight();

    for (const auto& edge : mesh.halfEdges)
    {
        if (edge.get() >= edge->twin)
        {
            continue;
        }

        Vertex* v1 = edge->start;
        Vertex* v2 = edge->end;

        DirectX::XMVECTOR p1 = DirectX::XMVectorSet(v1->x, v1->y, v1->z, 1.0f);
        DirectX::XMVECTOR p2 = DirectX::XMVectorSet(v2->x, v2->y, v2->z, 1.0f);

        uint32_t v1_x = 0, v1_y = 0;
        uint32_t v2_x = 0, v2_y = 0;

        if (ProjectToScreen(p1, modelViewProj, screenWidth, screenHeight, &v1_x, &v1_y) &&
            ProjectToScreen(p2, modelViewProj, screenWidth, screenHeight, &v2_x, &v2_y))
        {
            Draw::LineBresenham(renderer, v1_x, v1_y, v2_x, v2_y, 0xFFFFFFFF);
        }
    }
}
