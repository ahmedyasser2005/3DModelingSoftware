#include "Line.h"
#include "Renderer/Renderer.h"
#include <cmath>

void Draw::LineBresenham(Renderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    int32_t dx  = std::abs(x1 - x0);
    int32_t dy  = std::abs(y1 - y0);
    int32_t sx  = (x0 < x1) ? 1 : -1;
    int32_t sy  = (y0 < y1) ? 1 : -1;
    int32_t err = dx - dy;

    while (true)
    {
        renderer.PutPixel(x0, y0, r, g, b);

        if (x0 == x1 && y0 == y1)
            break;

        int32_t e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void Draw::LineFractionalAccumulation(
    Renderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    int32_t dx = std::abs(x1 - x0);
    int32_t dy = std::abs(y1 - y0);

    int32_t sx = (x0 < x1) ? 1 : -1;
    int32_t sy = (y0 < y1) ? 1 : -1;

    int32_t x = x0;
    int32_t y = y0;

    float accumulator = 0.0f;

    if (dx >= dy)
    {
        if (dx == 0)
        {
            renderer.PutPixel(x, y, r, g, b);
            return;
        }

        float slope = static_cast<float>(dy) / static_cast<float>(dx);

        for (int32_t i = 0; i <= dx; ++i)
        {
            renderer.PutPixel(x, y, r, g, b);

            x += sx;
            accumulator += slope;

            if (accumulator >= 0.5f)
            {
                y += sy;
                accumulator -= 1.0f;
            }
        }
    }
    else
    {
        float inverseSlope = static_cast<float>(dx) / static_cast<float>(dy);

        for (int32_t i = 0; i <= dy; ++i)
        {
            renderer.PutPixel(x, y, r, g, b);

            y += sy;
            accumulator += inverseSlope;

            if (accumulator >= 0.5f)
            {
                x += sx;
                accumulator -= 1.0f;
            }
        }
    }
}

void Draw::LineDDA(Renderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;

    int32_t steps = std::max(std::abs(dx), std::abs(dy));

    if (steps == 0)
    {
        renderer.PutPixel(x0, y0, r, g, b);
        return;
    }

    float xInc = dx / static_cast<float>(steps);
    float yInc = dy / static_cast<float>(steps);

    float x = static_cast<float>(x0);
    float y = static_cast<float>(y0);

    for (int32_t i = 0; i <= steps; ++i)
    {
        renderer.PutPixel(static_cast<int32_t>(std::round(x)), static_cast<int32_t>(std::round(y)), r, g, b);
        x += xInc;
        y += yInc;
    }
}
