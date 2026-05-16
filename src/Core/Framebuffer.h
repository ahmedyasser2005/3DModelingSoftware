#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>

/**
 * @class Framebuffer
 * @brief Represents a linear software pixel canvas configuration stored in system RAM.
 * * Provides highly optimized standard allocation models to draw structural primitives
 * before hardware context mapping.
 */
class [[nodiscard]] Framebuffer
{
  public:
    /**
     * @brief Instantiates a virtual software workspace array canvas layout.
     * @param width Target horizontal coordinate width map boundary.
     * @param height Target vertical coordinate height map boundary.
     */
    Framebuffer(uint32_t width, uint32_t height)
        : m_width(width), m_height(height), m_pixels(width * height, 0xFF000000)
    {
    }

    /**
     * @brief Plots a single pixel value to a localized grid destination coordinate.
     * * Includes rapid implicit bounds protection filtering out out-of-bounds metrics automatically.
     * * @param x Destination array horizon pixel index offset.
     * @param y Destination array vertical pixel index offset.
     * @param color Target raw 32-bit pixel value configuration (Format: 0xAARRGGBB).
     */
    inline void PutPixel(uint32_t x, uint32_t y, uint32_t color) noexcept
    {
        if (x >= m_width || y >= m_height) [[unlikely]]
            return;
        m_pixels[y * m_width + x] = color;
    }

    /**
     * @brief Reallocates structural linear memory alignment limits on context shifts.
     * @param width Updated structural array configuration tracking width metric.
     * @param height Updated structural array configuration tracking height metric.
     */
    void Resize(uint32_t width, uint32_t height) noexcept
    {
        m_width = width;
        m_height = height;
        m_pixels.assign(width * height, 0xFF000000);
    }

    /**
     * @brief Overwrites every linear pixel storage value with a uniform solid fill block.
     * @param color Target raw 32-bit clear color formatting (Defaults to opaque black).
     */
    void Clear(uint32_t color = 0xFF000000) noexcept
    {
        std::fill(m_pixels.begin(), m_pixels.end(), color);
    }

    /**
     * @brief Exposes the underlying sequential memory structure destination tracking elements.
     * @return Const raw read-only pointer to the base uint32_t element segment array.
     */
    [[nodiscard]] const uint32_t *GetRawData() const noexcept
    {
        return m_pixels.data();
    }

    /**
     * @brief Queries the structural width capacity of the active frame block.
     * @return Width as a unsigned integer unit.
     */
    [[nodiscard]] uint32_t GetWidth() const noexcept
    {
        return m_width;
    }

    /**
     * @brief Queries the structural height capacity of the active frame block.
     * @return Height as a unsigned integer unit.
     */
    [[nodiscard]] uint32_t GetHeight() const noexcept
    {
        return m_height;
    }

  private:
    uint32_t m_width;
    uint32_t m_height;
    std::vector<uint32_t> m_pixels;
};