#include "Draw.h"
#include "Renderer/SoftwareRenderer.h"
#include <cmath>

void Draw::LineBresenham(SoftwareRenderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    int32_t dx = std::abs(x1 - x0);
    int32_t dy = std::abs(y1 - y0);
    int32_t sx = (x0 < x1) ? 1 : -1;
    int32_t sy = (y0 < y1) ? 1 : -1;
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
    SoftwareRenderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
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

void Draw::LineDDA(SoftwareRenderer& renderer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
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

void Draw::CircleBresenham(SoftwareRenderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color)
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

void Draw::CircleMidpoint(SoftwareRenderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color)
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

void Draw::CirclePolar(SoftwareRenderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color)
{
    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    float dTheta = 1.0f / radius;
    float pi2 = 2.0f * 3.1415926535f;

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

void Draw::CircleCartesian(SoftwareRenderer& renderer, int32_t cx, int32_t cy, int32_t radius, uint32_t color)
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

void Draw::EllipseMidpoint(SoftwareRenderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color)
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
    SoftwareRenderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color)
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

void Draw::EllipsePolar(SoftwareRenderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color)
{
    if (rx <= 0 || ry <= 0)
        return;

    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    int32_t maxRadius = std::max(rx, ry);
    float dTheta = 1.0f / maxRadius;
    float piOver2 = 3.1415926535f / 2.0f;

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

void Draw::EllipseCartesian(SoftwareRenderer& renderer, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t color)
{
    if (rx <= 0 || ry <= 0)
        return;

    uint8_t r = static_cast<uint8_t>((color >> 16) & 0xFF);
    uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t b = static_cast<uint8_t>(color & 0xFF);

    for (int32_t x = 0; x <= rx; ++x)
    {
        float xNorm = static_cast<float>(x) / rx;
        int32_t y = static_cast<int32_t>(std::round(ry * std::sqrt(1.0f - xNorm * xNorm)));

        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);
    }

    for (int32_t y = 0; y <= ry; ++y)
    {
        float yNorm = static_cast<float>(y) / ry;
        int32_t x = static_cast<int32_t>(std::round(rx * std::sqrt(1.0f - yNorm * yNorm)));

        renderer.PutPixel(cx + x, cy + y, r, g, b);
        renderer.PutPixel(cx - x, cy + y, r, g, b);
        renderer.PutPixel(cx + x, cy - y, r, g, b);
        renderer.PutPixel(cx - x, cy - y, r, g, b);
    }
}
