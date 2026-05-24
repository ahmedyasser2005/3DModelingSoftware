#include "RationalBezier.h"
#include "Renderer/Renderer.h"
#include <cmath>
#include "Draw/Line.h"

void Draw::CurveRationalBezier(Renderer& renderer,
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
                               uint32_t color)
{
    int32_t steps = 64;
    int32_t t = 0;
    int32_t tStep = (1 << 10) / steps;

    int32_t prevX = (p0_x << 10) / w0;
    int32_t prevY = (p0_y << 10) / w0;

    for (int32_t i = 1; i <= steps; i++)
    {
        t += tStep;

        int32_t u = (1 << 10) - t;
        int32_t u2 = (u * u) >> 10;
        int32_t u3 = (u2 * u) >> 10;
        int32_t t2 = (t * t) >> 10;
        int32_t t3 = (t2 * t) >> 10;

        int32_t b0 = u3;
        int32_t b1 = (3 * u2 * t) >> 10;
        int32_t b2 = (3 * u * t2) >> 10;
        int32_t b3 = t3;

        int32_t xn = ((b0 * p0_x) + (b1 * p1_x) + (b2 * p2_x) + (b3 * p3_x)) >> 10;
        int32_t yn = ((b0 * p0_y) + (b1 * p1_y) + (b2 * p2_y) + (b3 * p3_y)) >> 10;
        int32_t wd = ((b0 * w0) + (b1 * w1) + (b2 * w2) + (b3 * w3)) >> 10;

        if (wd == 0)
            wd = 1; // Safety check

        int32_t x = (xn << 10) / wd;
        int32_t y = (yn << 10) / wd;

        // ONLY call LineBresenham. DO NOT call PutPixel here.
        Draw::LineBresenham(renderer, prevX, prevY, x, y, color);

        prevX = x;
        prevY = y;
    }
}