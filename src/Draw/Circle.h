#pragma once

#include <cstdint>

class Renderer;

namespace Draw
{
// Circle drawing - Fastest to slowest

void CircleBresenham(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

void CircleMidpoint(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

void CirclePolar(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

void CircleCartesian(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

} // namespace Draw