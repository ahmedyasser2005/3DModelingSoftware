#include "Renderer/Renderer.h"
#include <cmath>
#include "CubicBezier.h"
#include "Draw/Line.h"
#include "Helper.h"

void Draw::CurveCubicBezier(Renderer& renderer,
                            const DirectX::XMFLOAT4X4& viewProj, // Pass your Camera Matrix
                            DirectX::XMFLOAT3 p0,
                            DirectX::XMFLOAT3 p1,
                            DirectX::XMFLOAT3 p2,
                            DirectX::XMFLOAT3 p3,
                            uint32_t color)
{
    float sw = static_cast<float>(renderer.GetWidth());
    float sh = static_cast<float>(renderer.GetHeight());

    // Helper lambda to project 3D to 2D
    auto Project = [&](DirectX::XMFLOAT3 pos) -> DirectX::XMFLOAT2
    {
        DirectX::XMVECTOR point = DirectX::XMLoadFloat3(&pos);
        DirectX::XMMATRIX vp = DirectX::XMLoadFloat4x4(&viewProj);
        DirectX::XMVECTOR clipPos = DirectX::XMVector3TransformCoord(point, vp);

        DirectX::XMFLOAT3 clip;
        DirectX::XMStoreFloat3(&clip, clipPos);

        // Map to screen space
        return { (clip.x + 1.0f) * 0.5f * sw, (1.0f - clip.y) * 0.5f * sh };
    };

    int32_t steps = 64;
    DirectX::XMFLOAT2 prev = Project(p0);

    for (int32_t i = 1; i <= steps; i++)
    {
        float t = static_cast<float>(i) / static_cast<float>(steps);
        float u = 1.0f - t;

        // Cubic Bezier basis functions
        float b0 = u * u * u;
        float b1 = 3 * u * u * t;
        float b2 = 3 * u * t * t;
        float b3 = t * t * t;

        float x = b0 * p0.x + b1 * p1.x + b2 * p2.x + b3 * p3.x;
        float y = b0 * p0.y + b1 * p1.y + b2 * p2.y + b3 * p3.y;
        float z = b0 * p0.z + b1 * p1.z + b2 * p2.z + b3 * p3.z;

        DirectX::XMFLOAT2 curr = Project({ x, y, z });

        // Use LineBresenham instead of PutPixel
        Draw::LineBresenham(renderer,
                            static_cast<int32_t>(prev.x),
                            static_cast<int32_t>(prev.y),
                            static_cast<int32_t>(curr.x),
                            static_cast<int32_t>(curr.y),
                            color);

        prev = curr;
    }
}