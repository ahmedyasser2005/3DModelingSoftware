#pragma once

#include <cstdint>

class Renderer;

namespace Draw
{

void CurveRationalBezier(Renderer& renderer,
                         int32_t p0_x,
                         int32_t p0_y,
                         int32_t w0,
                         int32_t p1_x,
                         int32_t p1_y,
                         int32_t w1,
                         int32_t p2_x,
                         int32_t p2_y,
                         int32_t w2,
                         int32_t p3_x,
                         int32_t p3_y,
                         int32_t w3,
                         uint32_t color);

} // namespace Draw