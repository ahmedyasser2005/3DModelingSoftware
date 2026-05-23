#pragma once

#include <cstdint>

class Renderer;

namespace Draw
{

// Line drawing - Fastest to slowest

void LineBresenham(Renderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);

void LineFractionalAccumulation(
    Renderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);

void LineDDA(Renderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);

// Circle drawing - Fastest to slowest

void CircleBresenham(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

void CircleMidpoint(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

void CirclePolar(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

void CircleCartesian(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

// Ellipse drawing - Fastest to slowest

void EllipseMidpoint(Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

void EllipseFractionalAccumulation(
    Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

void EllipsePolar(Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

void EllipseCartesian(Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

} // namespace Draw