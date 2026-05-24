#include "Circle.h"
#include "Renderer/Renderer.h"
#include <cmath>

void Draw::CircleBresenham(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color)
{
    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    int32_t x = 0;
    int32_t y = radius;
    int32_t d = 3 - 2 * radius;

    while (y >= x)
    {
        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);
        renderer.PutPixel(cx + y, cy + x, r, g, b);
        renderer.PutPixel(cx - y, cy + x, r, g, b);
        renderer.PutPixel(cx + y, cy - x, r, g, b);
        renderer.PutPixel(cx - y, cy - x, r, g, b);

        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void Draw::CircleMidpoint(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color)
{
    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    int32_t x = 0;
    int32_t y = radius;
    int32_t d = 3 - 2 * radius;

    while (y >= x)
    {
        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);
        renderer.PutPixel(cx + y, cy + x, r, g, b);
        renderer.PutPixel(cx - y, cy + x, r, g, b);
        renderer.PutPixel(cx + y, cy - x, r, g, b);
        renderer.PutPixel(cx - y, cy - x, r, g, b);

        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}

void Draw::CirclePolar(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color)
{
    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    float dTheta = 1.0f / radius;
    float pi2    = 2.0f * 3.1415926535f;

    for (float theta = 0.0f; theta <= pi2 / 4.0f; theta += dTheta)
    {
        int32_t x = static_cast<int32_t>(std::round(radius * std::cos(theta)));
        int32_t y = static_cast<int32_t>(std::round(radius * std::sin(theta)));

        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);
        renderer.PutPixel(cx + y, cy + x, r, g, b);
        renderer.PutPixel(cx - y, cy + x, r, g, b);
        renderer.PutPixel(cx + y, cy - x, r, g, b);
        renderer.PutPixel(cx - y, cy - x, r, g, b);
    }
}

void Draw::CircleCartesian(Renderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color)
{
    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    for (int32_t x = 0; x <= radius; ++x)
    {
        int32_t y = static_cast<int32_t>(std::round(std::sqrt(radius * radius - x * x)));

        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);
        renderer.PutPixel(cx + y, cy + x, r, g, b);
        renderer.PutPixel(cx - y, cy + x, r, g, b);
        renderer.PutPixel(cx + y, cy - x, r, g, b);
        renderer.PutPixel(cx - y, cy - x, r, g, b);
    }
}
