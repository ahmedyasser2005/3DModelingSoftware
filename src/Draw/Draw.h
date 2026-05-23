#pragma once

#include <cstdint>

class SoftwareRenderer;

namespace Draw
{

// Line drawing - Fastest to slowest

void LineBresenham(SoftwareRenderer* renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);

void LineFractionalAccumulation(SoftwareRenderer* renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);

void LineDDA(SoftwareRenderer* renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);

// Circle drawing - Fastest to slowest

void CircleBresenham(SoftwareRenderer* renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

void CircleMidpoint(SoftwareRenderer* renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

void CirclePolar(SoftwareRenderer* renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

void CircleCartesian(SoftwareRenderer* renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color);

// Ellipse drawing - Fastest to slowest

void EllipseMidpoint(SoftwareRenderer* renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

void EllipseFractionalAccumulation(SoftwareRenderer* renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

void EllipsePolar(SoftwareRenderer* renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

void EllipseCartesian(SoftwareRenderer* renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color);

}