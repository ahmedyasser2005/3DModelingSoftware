#pragma once

#include <cstdint>
#include <vector>

class SoftwareRenderer final
{
  public:
    SoftwareRenderer() = default;
    ~SoftwareRenderer() = default;
    SoftwareRenderer(const SoftwareRenderer&) = delete;
    SoftwareRenderer& operator=(const SoftwareRenderer&) = delete;
    SoftwareRenderer(SoftwareRenderer&&) = delete;
    SoftwareRenderer& operator=(SoftwareRenderer&&) = delete;

    void Initialize(uint32_t width, uint32_t height);
    void Resize(uint32_t width, uint32_t height);

    void Clear(uint8_t r, uint8_t g, uint8_t b);
    void PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

    [[nodiscard]] const std::vector<uint32_t>& GetFramebuffer() const noexcept
    {
        return m_Framebuffer;
    }

    [[nodiscard]] uint32_t GetWidth() const noexcept
    {
        return m_Width;
    }
    [[nodiscard]] uint32_t GetHeight() const noexcept
    {
        return m_Height;
    }

  private:
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    std::vector<uint32_t> m_Framebuffer; // 0x (AA forced to be 'FF') RR GG BB
};