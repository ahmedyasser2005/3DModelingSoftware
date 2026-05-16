#include "Application.h"
#include "Framebuffer.h"
#include "Rasterizer.h"
#include "Renderer.h"
#include "Window.h"
#include <array>
#include <cmath>

static constexpr int Width = 1280;
static constexpr int Height = 720;
static constexpr const wchar_t *Title = L"3D Modeling Software";

struct Application::AppImpl
{
    Window window;
    Renderer renderer;
    Framebuffer framebuffer;

    AppImpl() : window(Width, Height, Title), renderer(window.GetNativeHandle()), framebuffer(Width, Height)
    {
    }
};

static void RenderFrame(Framebuffer &fb, Renderer &renderer)
{
    fb.Clear(0xFF0B0B0E);

    DrawBresenhamLine(fb, 100, 100, 500, 400, 0xFFFFFFFF);

    uint32_t w = fb.GetWidth();
    uint32_t h = fb.GetHeight();

    renderer.CopyFramebufferToScreen(fb.GetRawData(), w, h);
    renderer.Present();
}

Application::Application() : m_impl(std::make_unique<AppImpl>())
{
    m_impl->window.SetResizeCallback([this](uint32_t width, uint32_t height) {
        if (width > 0 && height > 0)
        {
            m_impl->framebuffer.Resize(width, height);
            m_impl->renderer.Resize(width, height);
            RenderFrame(m_impl->framebuffer, m_impl->renderer);
        }
    });
}

Application::~Application() = default;

int Application::Run()
{
    while (!m_impl->window.ShouldClose()) [[likely]]
    {
        m_impl->window.ProcessMessages();

        if (m_impl->window.ConsumeResizeFlag())
        {
            auto [w, h] = m_impl->window.GetDimensions();
            if (w > 0 && h > 0)
            {
                m_impl->framebuffer.Resize(w, h);
                m_impl->renderer.Resize(w, h);
            }
        }

        RenderFrame(m_impl->framebuffer, m_impl->renderer);
    }
    return 0;
}