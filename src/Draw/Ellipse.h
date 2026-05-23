#pragma once

#include <cstdint>

class Renderer;

namespace Draw
{
// Ellipse drawing - Fastest to slowest

void EllipseMidpoint(Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

void EllipseFractionalAccumulation(Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

void EllipsePolar(Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

void EllipseCartesian(Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

} // namespace Draw