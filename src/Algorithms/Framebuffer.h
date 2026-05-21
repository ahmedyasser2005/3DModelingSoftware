#pragma once
#include <cstdint>
#include <vector>

class Framebuffer
{
  public:
    Framebuffer(uint32_t width, uint32_t height) : m_width(width), m_height(height), m_pixels(width * height, 0)
    {
    }

    inline void PutPixel(uint32_t x, uint32_t y, uint32_t color) noexcept
    {
        if (x >= m_width || y >= m_height) [[unlikely]]
        {
            return;
        }
        m_pixels[y * m_width + x] = color;
    }

    [[nodiscard]] inline uint32_t* GetData() noexcept
    {
        return m_pixels.data();
    }

    [[nodiscard]] inline uint32_t GetWidth() const noexcept
    {
        return m_width;
    }

    [[nodiscard]] inline uint32_t GetHeight() const noexcept
    {
        return m_height;
    }

  private:
    uint32_t m_width;
    uint32_t m_height;
    std::vector<uint32_t> m_pixels;
};