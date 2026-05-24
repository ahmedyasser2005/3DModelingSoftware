#include "Application.h"
#include <DirectXMath.h>
#include <chrono>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#define k_EditorBg 30, 33, 40 // Sleek editor dark mode background
using Clock = std::chrono::high_resolution_clock;

Application::Application() : m_Window({ L"3D Modeling Software Engine", 1280, 720 })
{
    m_DX11Presenter.Initialize(m_Window.GetNativeHandle(), m_Window.GetWidth(), m_Window.GetHeight());
    m_Renderer.Initialize(m_Window.GetWidth(), m_Window.GetHeight());
    m_SceneGraph.Initialize(m_Renderer);
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

        auto  now     = Clock::now();
        float dt      = std::chrono::duration<float>(now - lastFrameTime).count();
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

void Application::Update(const float& deltaTime)
{
    (void)deltaTime;
    const InputHandler& InputHandler = m_Window.GetInputHandler();
    ImGuiIO&            io           = ImGui::GetIO();

    if (io.WantCaptureKeyboard || io.WantCaptureMouse)
        return;

    if (InputHandler.IsKeyDown(KeyCode::Escape))
    {
        m_IsRunning = false;
        return;
    }
}

void Application::Render()
{
    m_Renderer.Clear(k_EditorBg);

    m_SceneGraph.Render();

    // Add EditorUI drawing functons here like this:
    // Example -> m_EditorUI.CreateMainMenu();
}

void Application::OnResize(uint32_t w, uint32_t h)
{
    m_Renderer.ResizeCanvas(w, h, k_EditorBg);
    m_DX11Presenter.Resize(w, h);
    m_DX11Presenter.Present(m_Renderer.GetFramebuffer(), !m_ResizePending);
}