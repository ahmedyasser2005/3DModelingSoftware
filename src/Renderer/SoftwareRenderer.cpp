#include "SoftwareRenderer.h"
#include <algorithm>

void SoftwareRenderer::Initialize(uint32_t width, uint32_t height)
{
    Resize(width, height);
}

void SoftwareRenderer::Resize(uint32_t width, uint32_t height)
{
    m_Width = width;
    m_Height = height;
    m_Framebuffer.resize(static_cast<size_t>(m_Width) * m_Height);
}

void SoftwareRenderer::Clear(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t clearColor = (255 << 24) | (r << 16) | (g << 8) | b;
    std::fill(m_Framebuffer.begin(), m_Framebuffer.end(), clearColor);
}

void SoftwareRenderer::PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x >= m_Width || y >= m_Height)
        return;
    m_Framebuffer[static_cast<size_t>(y) * m_Width + x] = (255 << 24) | (r << 16) | (g << 8) | b;
}
