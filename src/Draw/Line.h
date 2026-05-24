#pragma once

#include <cstdint>

class Renderer;

namespace Draw
{
// Line drawing - Fastest to slowest

void LineBresenham(Renderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);

void LineFractionalAccumulation(Renderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);

void LineDDA(Renderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);

} // namespace Draw