#include "Ellipse.h"
#include "Renderer/Renderer.h"
#include <cmath>

void Draw::EllipseMidpoint(Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color)
{
    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    int32_t rx2 = rx * rx;
    int32_t ry2 = ry * ry;

    int32_t x = 0;
    int32_t y = ry;

    int32_t px = 0;
    int32_t py = 2 * rx2 * y;

    int32_t p = static_cast<int32_t>(std::round(ry2 - (rx2 * ry) + (0.25f * rx2)));
    while (px < py)
    {
        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);

        x++;
        px += 2 * ry2;
        if (p < 0)
        {
            p += ry2 + px;
        }
        else
        {
            y--;
            py -= 2 * rx2;
            p += ry2 + px - py;
        }
    }

    p = static_cast<int32_t>(std::round(ry2 * (x + 0.5f) * (x + 0.5f) + rx2 * (y - 1) * (y - 1) - rx2 * ry2));
    while (y >= 0)
    {
        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);

        y--;
        py -= 2 * rx2;
        if (p > 0)
        {
            p += rx2 - py;
        }
        else
        {
            x++;
            px += 2 * ry2;
            p += rx2 + px - py;
        }
    }
}

void Draw::EllipseFractionalAccumulation(
    Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color)
{
    if (rx <= 0 || ry <= 0)
        return;

    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    int32_t rx2 = rx * rx;
    int32_t ry2 = ry * ry;

    int32_t x = 0;
    int32_t y = ry;

    int32_t px = 0;
    int32_t py = 2 * rx2 * y;

    float accumulator = 0.0f;

    while (px < py)
    {
        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);

        x++;
        px += 2 * ry2;

        float slope = (static_cast<float>(ry2) * x) / (static_cast<float>(rx2) * y);
        accumulator += slope;

        if (accumulator >= 0.5f)
        {
            y--;
            py -= 2 * rx2;
            accumulator -= 1.0f;
        }
    }

    accumulator = 0.0f;

    while (y >= 0)
    {
        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);

        y--;
        py -= 2 * rx2;

        float inverseSlope = 0.0f;
        if (x != 0)
        {
            inverseSlope = (static_cast<float>(rx2) * y) / (static_cast<float>(ry2) * x);
        }
        accumulator += inverseSlope;

        if (accumulator >= 0.5f)
        {
            x++;
            px += 2 * ry2;
            accumulator -= 1.0f;
        }
    }
}

void Draw::EllipsePolar(Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color)
{
    if (rx <= 0 || ry <= 0)
        return;

    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    int32_t maxRadius = std::max(rx, ry);
    float   dTheta    = 1.0f / maxRadius;
    float   piOver2   = 3.1415926535f / 2.0f;

    for (float theta = 0.0f; theta <= piOver2; theta += dTheta)
    {
        int32_t x = static_cast<int32_t>(std::round(rx * std::cos(theta)));
        int32_t y = static_cast<int32_t>(std::round(ry * std::sin(theta)));

        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);
    }
}

void Draw::EllipseCartesian(Renderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color)
{
    if (rx <= 0 || ry <= 0)
        return;

    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    for (int32_t x = 0; x <= rx; ++x)
    {
        float   xNorm = static_cast<float>(x) / rx;
        int32_t y     = static_cast<int32_t>(std::round(ry * std::sqrt(1.0f - xNorm * xNorm)));

        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);
    }

    for (int32_t y = 0; y <= ry; ++y)
    {
        float   yNorm = static_cast<float>(y) / ry;
        int32_t x     = static_cast<int32_t>(std::round(rx * std::sqrt(1.0f - yNorm * yNorm)));

        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);
    }
}
