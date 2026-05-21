#pragma once
#include "Framebuffer.h"
#include <cmath>
#include <algorithm>

class Rasterizer
{
public:
    static void DrawBresenhamLine(Framebuffer& framebuffer, int x0, int y0, int x1, int y1, uint32_t color) noexcept
    {
        const int width = static_cast<int>(framebuffer.GetWidth());
        const int height = static_cast<int>(framebuffer.GetHeight());

        if ((x0 < 0 && x1 < 0) || (x0 >= width && x1 >= width) ||
            (y0 < 0 && y1 < 0) || (y0 >= height && y1 >= height)) [[unlikely]]
        {
            return;
        }

        const int deltaX = std::abs(x1 - x0);
        const int deltaY = std::abs(y1 - y0);
        const int stepX = (x0 < x1) ? 1 : -1;
        const int stepY = (y0 < y1) ? 1 : -1;
        int error = deltaX - deltaY;

        uint32_t* const data = framebuffer.GetData();
        const int stride = width;

        while (true)
        {
            if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) [[likely]]
            {
                data[y0 * stride + x0] = color;
            }

            if (x0 == x1 && y0 == y1)
            {
                break;
            }
            
            const int error2 = 2 * error;
            if (error2 > -deltaY)
            {
                error -= deltaY;
                x0 += stepX;
            }
            if (error2 < deltaX)
            {
                error += deltaX;
                y0 += stepY;
            }
        }
    }

    static void DrawCircle(Framebuffer& framebuffer, int xCenter, int yCenter, int radius, uint32_t color) noexcept
    {
        const int width = static_cast<int>(framebuffer.GetWidth());
        const int height = static_cast<int>(framebuffer.GetHeight());

        if (xCenter + radius < 0 || xCenter - radius >= width || yCenter + radius < 0 || yCenter - radius >= height) [[unlikely]]
        {
            return;
        }

        int x = 0;
        int y = radius;
        int decision = 3 - 2 * radius;

        uint32_t* const data = framebuffer.GetData();
        const int stride = width;

        auto plot = [data, stride, width, height, color](int pixelX, int pixelY) inline noexcept
        {
            if (pixelX >= 0 && pixelX < width && pixelY >= 0 && pixelY < height) [[likely]]
            {
                data[pixelY * stride + pixelX] = color;
            }
        };

        while (y >= x)
        {
            plot(xCenter + x, yCenter + y);
            plot(xCenter - x, yCenter + y);
            plot(xCenter + x, yCenter - y);
            plot(xCenter - x, yCenter - y);
            plot(xCenter + y, yCenter + x);
            plot(xCenter - y, yCenter + x);
            plot(xCenter + y, yCenter - x);
            plot(xCenter - y, yCenter - x);

            if (decision > 0)
            {
                y--;
                decision = decision + 4 * (x - y) + 10;
            }
            else
            {
                decision = decision + 4 * x + 6;
            }
            x++;
        }
    }
};