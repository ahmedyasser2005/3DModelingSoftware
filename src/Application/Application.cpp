#include "Application.h"
#include "Renderer/TestMesh.h" // Testing
#include <DirectXMath.h>
#include <chrono>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

struct ColorRGB
{
    uint8_t r, g, b;
};
inline constexpr ColorRGB k_EditorBg{ 30, 33, 40 }; // Sleek editor dark mode background

using Clock = std::chrono::high_resolution_clock;

Application::Application() : m_Window({ L"3D Modeling Software Engine", 1280, 720 })
{
    m_DX11Presenter.Initialize(m_Window.GetNativeHandle(), m_Window.GetWidth(), m_Window.GetHeight());
    m_Renderer.Initialize(m_Window.GetWidth(), m_Window.GetHeight());
    m_SceneGraph.Initialize(m_Renderer);
    m_EditorUI.Initialize(m_Window.GetNativeHandle(), m_DX11Presenter.GetDevice(), m_DX11Presenter.GetContext());

    m_Renderer.Clear(k_EditorBg.r, k_EditorBg.g, k_EditorBg.b);

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

    FirstFrame();

    while (m_IsRunning)
    {
        HandleEvents();

        auto  now     = Clock::now();
        float dt      = std::chrono::duration<float>(now - lastFrameTime).count();
        lastFrameTime = now;

        Update(dt);

        m_Renderer.Clear(k_EditorBg.r, k_EditorBg.g, k_EditorBg.b);

        Render();
        m_Window.GetInputHandler().ClearMouseWheelDelta();

        m_EditorUI.StartFrame();
        RenderUI();
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

void Application::FirstFrame()
{
    // NOTE: This is AI generated code.
    // Only used for testing purposes.
    // There will be different implementation of this.
    auto cubeNode      = std::make_unique<Node>();
    cubeNode->Name     = "TestCube";
    cubeNode->Mesh     = CreatePrimitiveCube(1.0f);
    cubeNode->Position = { 0.0f, 0.0f, 0.0f };
    m_SceneGraph.GetRoot()->Children.emplace_back(std::move(cubeNode));
}

void Application::Update(const float& deltaTime)
{
    InputHandler& input = m_Window.GetInputHandler();
    ImGuiIO&      io    = ImGui::GetIO();

    DirectX::XMFLOAT4X4 identityMatrix;
    DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());

    // Handle Input
    if (input.IsKeyDown(KeyCode::Escape)) [[unlikely]]
    {
        m_IsRunning = false;
        return;
    }

    // Handle ImGui
    if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    {
        return;
    }

    // Update Camera
    m_Camera.Update(input, (float)m_Renderer.GetWidth(), (float)m_Renderer.GetHeight(), deltaTime);

    // Update Scene Objects
    m_SceneGraph.Update(m_SceneGraph.GetRoot(), identityMatrix);
    m_SceneGraph.SetViewProjMatrix(m_Camera.GetViewProjMatrix());
}

void Application::Render()
{
    m_SceneGraph.Render();
}

void Application::RenderUI()
{
    // Add EditorUI drawing functons here like this:
    // Example -> m_EditorUI.CreateMainMenu();
}

void Application::OnResize(uint32_t width, uint32_t height)
{
    m_Renderer.ResizeCanvas(width, height, k_EditorBg.r, k_EditorBg.g, k_EditorBg.b);
    m_DX11Presenter.Resize(width, height);
    m_DX11Presenter.Present(m_Renderer.GetFramebuffer(), !m_ResizePending);
}