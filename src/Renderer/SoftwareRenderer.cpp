#include "SoftwareRenderer.h"
#include <algorithm>
#include <cstring>

void SoftwareRenderer::RebuildFramebuffer()
{
    m_Framebuffer.resize(static_cast<size_t>(m_Width) * m_Height);

    for (uint32_t y = 0; y < m_Height; ++y)
    {
        const uint32_t* src = m_Canvas.data() + y * m_CanvasW;
        uint32_t* dst = m_Framebuffer.data() + y * m_Width;
        std::memcpy(dst, src, static_cast<size_t>(m_Width) * sizeof(uint32_t));
    }
}

void SoftwareRenderer::Initialize(uint32_t width, uint32_t height)
{
    Resize(width, height);
}

void SoftwareRenderer::Resize(uint32_t width, uint32_t height)
{
    m_Width = width;
    m_Height = height;
    m_CanvasW = width;
    m_CanvasH = height;

    m_Canvas.assign(static_cast<size_t>(m_CanvasW) * m_CanvasH, 0xFF000000u);
    m_Framebuffer.assign(static_cast<size_t>(m_Width) * m_Height, 0xFF000000u);
}

void SoftwareRenderer::ResizeCanvas(uint32_t newW, uint32_t newH, uint8_t bgR, uint8_t bgG, uint8_t bgB)
{
    m_Width = newW;
    m_Height = newH;

    const uint32_t bgColor = (255u << 24) | (static_cast<uint32_t>(bgR) << 16) | (static_cast<uint32_t>(bgG) << 8) |
                             static_cast<uint32_t>(bgB);

    bool canvasGrew = (newW > m_CanvasW) || (newH > m_CanvasH);

    if (canvasGrew)
    {
        const uint32_t expandW = std::max(m_CanvasW, newW);
        const uint32_t expandH = std::max(m_CanvasH, newH);

        std::vector<uint32_t> newCanvas(static_cast<size_t>(expandW) * expandH, bgColor);

        for (uint32_t y = 0; y < m_CanvasH; ++y)
        {
            const uint32_t* src = m_Canvas.data() + y * m_CanvasW;
            uint32_t* dst = newCanvas.data() + y * expandW;
            std::memcpy(dst, src, static_cast<size_t>(m_CanvasW) * sizeof(uint32_t));
        }

        m_Canvas = std::move(newCanvas);
        m_CanvasW = expandW;
        m_CanvasH = expandH;
    }

    RebuildFramebuffer();
}

void SoftwareRenderer::Clear(uint8_t r, uint8_t g, uint8_t b)
{
    const uint32_t color =
        (255u << 24) | (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(b);

    std::fill(m_Canvas.begin(), m_Canvas.end(), color);

    std::fill(m_Framebuffer.begin(), m_Framebuffer.end(), color);
}

void SoftwareRenderer::PutPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x < 0 || y < 0 || static_cast<uint32_t>(x) >= m_Width || static_cast<uint32_t>(y) >= m_Height)
        return;

    const uint32_t color =
        (255u << 24) | (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(b);

    m_Canvas[static_cast<size_t>(y) * m_CanvasW + static_cast<size_t>(x)] = color;

    m_Framebuffer[static_cast<size_t>(y) * m_Width + static_cast<size_t>(x)] = color;
}
