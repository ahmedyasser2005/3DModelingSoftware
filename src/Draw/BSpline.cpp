#include "BSpline.h"
#include <cmath>
#include <vector>
#include "Draw/Line.h"

void Draw::CurveBSpline(Renderer& renderer,
                        const std::vector<DirectX::XMFLOAT3>& points,
                        uint32_t color,
                        int segmentsPerCurve)
{
    if (points.size() < 4)
        return;

    for (size_t i = 0; i < points.size() - 3; ++i)
    {
        const auto& p0 = points[i];
        const auto& p1 = points[i + 1];
        const auto& p2 = points[i + 2];
        const auto& p3 = points[i + 3];

        float prevX = p0.x;
        float prevY = p0.y;

        for (int j = 1; j <= segmentsPerCurve; j++)
        {
            float t = static_cast<float>(j) / static_cast<float>(segmentsPerCurve);
            float t2 = t * t;
            float t3 = t2 * t;

            float b0 = (1.0f - 3.0f * t + 3.0f * t2 - t3) / 6.0f;
            float b1 = (4.0f - 6.0f * t2 + 3.0f * t3) / 6.0f;
            float b2 = (1.0f + 3.0f * t + 3.0f * t2 - 3.0f * t3) / 6.0f;
            float b3 = t3 / 6.0f;

            float x = b0 * p0.x + b1 * p1.x + b2 * p2.x + b3 * p3.x;
            float y = b0 * p0.y + b1 * p1.y + b2 * p2.y + b3 * p3.y;

            Draw::LineBresenham(renderer,
                                static_cast<int32_t>(prevX),
                                static_cast<int32_t>(prevY),
                                static_cast<int32_t>(x),
                                static_cast<int32_t>(y),
                                color);

            prevX = x;
            prevY = y;
        }
    }
}