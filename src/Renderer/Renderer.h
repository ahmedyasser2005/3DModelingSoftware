#pragma once

#include <cstdint>
#include <vector>

// Two-buffer design:
//   m_Canvas       — the full scene backing store. Only grows. Coordinates are
//                    in canvas space and may exceed the current viewport size.
//
//   m_Framebuffer  — viewport-sized slice of the canvas sent to DX11 each
//                    frame. Rebuilt by RebuildFramebuffer() after any resize.
//
// This supports future pan/zoom: when the user zooms out, the viewport
// (m_Width x m_Height) shows a subset of the canvas without reallocation.

class Renderer final
{
  public:
    Renderer() = default;
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void Initialize(uint32_t width, uint32_t height);
    void Resize(uint32_t width, uint32_t height);
    void ResizeCanvas(uint32_t width, uint32_t height, uint8_t bgR, uint8_t bgG, uint8_t bgB);

    void Clear(uint8_t r, uint8_t g, uint8_t b);
    void PutPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

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
    void RebuildFramebuffer();

    std::vector<uint32_t> m_Canvas;
    uint32_t m_CanvasW = 0;
    uint32_t m_CanvasH = 0;

    std::vector<uint32_t> m_Framebuffer;
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
};
