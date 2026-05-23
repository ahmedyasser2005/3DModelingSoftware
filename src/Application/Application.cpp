#include "Application.h"
#include <chrono>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#define k_EditorBg 30, 33, 40 // Sleek editor dark mode background
using Clock = std::chrono::high_resolution_clock;

Application::Application() : m_Window({ L"3D Modeling Software Engine", 1280, 720 })
{
    m_Renderer.Initialize(m_Window.GetWidth(), m_Window.GetHeight());
    m_DX11Presenter.Initialize(m_Window.GetNativeHandle(), m_Window.GetWidth(), m_Window.GetHeight());
    m_EditorUI.Initialize(m_Window.GetNativeHandle(), m_DX11Presenter.GetDevice(), m_DX11Presenter.GetContext());

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
    auto lastFrameTime = Clock::now();

    while (m_IsRunning)
    {
        HandleEvents();
        if (!m_IsRunning)
            break;

        auto now = Clock::now();
        float dt = std::chrono::duration<float>(now - lastFrameTime).count();
        lastFrameTime = now;

        Update(dt);

        m_EditorUI.StartFrame();
        Render();
        m_EditorUI.EndFrame();

        m_DX11Presenter.Present(m_Renderer.GetFramebuffer(), !m_ResizePending);
        m_ResizePending = false;
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
    ImGuiIO& io = ImGui::GetIO();

    // If ImGui is focused on a text box or widget,
    // ignore keyboard inputs for app logic
    if (io.WantCaptureKeyboard)
        return;
    // If ImGui is hovering over or dragging a window,
    // ignore mouse inputs for canvas painting
    if (io.WantCaptureMouse)
        return;

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

        m_Renderer.PutPixel(mx, my, 255, 200, 0);
        m_Renderer.PutPixel(mx + 1, my, 255, 200, 0);
        m_Renderer.PutPixel(mx, my + 1, 255, 200, 0);
        m_Renderer.PutPixel(mx + 1, my + 1, 255, 200, 0);
    }
}

void Application::Render()
{
    // Add EditorUI drawing functons here like this:
    // Example -> m_EditorUI.CreateMainMenu();
}

void Application::OnResize(uint32_t w, uint32_t h)
{
    m_Renderer.ResizeCanvas(w, h, k_EditorBg);
    m_DX11Presenter.Resize(w, h);
    m_DX11Presenter.Present(m_Renderer.GetFramebuffer(), !m_ResizePending);
}
