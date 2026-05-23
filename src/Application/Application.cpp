#include "Application.h"
#include <chrono>

#define k_EditorBg 30, 33, 40 // Sleek editor dark mode background
using Clock = std::chrono::high_resolution_clock;

Application::Application() : m_Window({ L"3D Modeling Software Engine", 1280, 720 })
{
    m_Renderer.Initialize(m_Window.GetWidth(), m_Window.GetHeight());
    m_DX11Presenter.Initialize(m_Window.GetNativeHandle(), m_Window.GetWidth(), m_Window.GetHeight());

    m_Renderer.Clear(k_EditorBg);

    m_Window.SetResizeCallback(
        [this](uint32_t w, uint32_t h)
        {
            OnResize(w, h);
            m_ResizePending = true;
        });
}

void Application::Run()
{
    while (m_IsRunning)
    {
        HandleEvents();
        if (!m_IsRunning)
            break;

        Clock::time_point m_LastFrameTime = Clock::now();
        auto now = Clock::now();
        float dt = std::chrono::duration<float>(now - m_LastFrameTime).count();
        m_LastFrameTime = now;
        Update(dt);

        Render();
    }
}

void Application::HandleEvents()
{
    if (!m_Window.ProcessMessages())
    {
        m_IsRunning = false;
        return;
    }

    uint32_t ww = m_Window.GetWidth();
    uint32_t wh = m_Window.GetHeight();
    if (ww > 0 && wh > 0 && (ww != m_Renderer.GetWidth() || wh != m_Renderer.GetHeight()))
    {
        OnResize(ww, wh);
    }
}

void Application::Update(const float& dt)
{
    (void)dt;

    const InputHandler& InputHandler = m_Window.GetInputHandler();

    if (InputHandler.IsKeyDown(KeyCode::Escape))
    {
        m_IsRunning = false;
        return;
    }

    if (InputHandler.IsKeyDown(KeyCode::Space))
    {
        m_Renderer.Clear(k_EditorBg);
    }

    if (InputHandler.IsMouseButtonDown(MouseButton::Left))
    {
        int32_t mx = InputHandler.GetMouseX();
        int32_t my = InputHandler.GetMouseY();

        // Draw gold pixels using a steady 2x2 brush layout
        m_Renderer.PutPixel(mx, my, 255, 200, 0);
        m_Renderer.PutPixel(mx + 1, my, 255, 200, 0);
        m_Renderer.PutPixel(mx, my + 1, 255, 200, 0);
        m_Renderer.PutPixel(mx + 1, my + 1, 255, 200, 0);
    }
}

void Application::Render()
{
    m_DX11Presenter.Present(m_Renderer.GetFramebuffer(), !m_ResizePending);
    m_ResizePending = false;
}

void Application::OnResize(uint32_t w, uint32_t h)
{
    m_Renderer.ResizeCanvas(w, h, k_EditorBg);
    m_DX11Presenter.Resize(w, h);
    m_DX11Presenter.Present(m_Renderer.GetFramebuffer(), !m_ResizePending);
}
