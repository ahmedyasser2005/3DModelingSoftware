#include "Application.h"
#include "Draw/BSpline.h"
#include "Draw/CubicBezier.h"
#include "Draw/RationalBezier.h"
#include "Renderer/TestMesh.h"
#include <DirectXMath.h>
#include <chrono>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

struct ColorRGB
{
    uint8_t r, g, b;
};
inline constexpr ColorRGB k_EditorBg{ 30, 33, 40 };

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

        auto now = Clock::now();
        float dt = std::chrono::duration<float>(now - lastFrameTime).count();
        lastFrameTime = now;

        Update(dt);

        m_Renderer.Clear(k_EditorBg.r, k_EditorBg.g, k_EditorBg.b);
        Render();
        m_Window.GetInputHandler().ClearMouseWheelDelta();
        RenderUI();

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
    auto cubeNode = std::make_unique<Node>();
    cubeNode->Name = "TestCube";
    cubeNode->Mesh = CreatePrimitiveCube(1.0f);
    cubeNode->Position = { 0.0f, 0.0f, 0.0f };
    m_SceneGraph.GetRoot()->Children.emplace_back(std::move(cubeNode));
}

void Application::Update(const float& deltaTime)
{
    InputHandler& input = m_Window.GetInputHandler();
    ImGuiIO& io = ImGui::GetIO();

    DirectX::XMFLOAT4X4 identityMatrix;
    DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());

    if (input.IsKeyDown(KeyCode::Escape)) [[unlikely]]
    {
        m_IsRunning = false;
        return;
    }

    if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    {
        return;
    }

    m_Camera.Update(input, (float)m_Renderer.GetWidth(), (float)m_Renderer.GetHeight(), deltaTime);
    m_SceneGraph.Update(m_SceneGraph.GetRoot(), identityMatrix);
    m_SceneGraph.SetViewProjMatrix(m_Camera.GetViewProjMatrix());
}

void Application::Render()
{
    DirectX::XMFLOAT4X4 vp = m_Camera.GetViewProjMatrix();

    uint8_t r = static_cast<uint8_t>(m_CurveState.Color[0] * 255.0f);
    uint8_t g = static_cast<uint8_t>(m_CurveState.Color[1] * 255.0f);
    uint8_t b = static_cast<uint8_t>(m_CurveState.Color[2] * 255.0f);
    uint32_t hexColor = (r << 16) | (g << 8) | b;

    Draw::CurveCubicBezier(
        m_Renderer, vp, m_CurveState.P0, m_CurveState.P1, m_CurveState.P2, m_CurveState.P3, hexColor);
}

void Application::RenderUI()
{
    m_EditorUI.StartFrame();
    m_EditorUI.BeginMasterDockspace();

    Node* cubeNode = nullptr;
    for (auto& child : m_SceneGraph.GetRoot()->Children)
    {
        if (child && child->Name == "TestCube")
        {
            cubeNode = child.get();
            break;
        }
    }

    m_EditorUI.ShowEditorPanels(m_Renderer, cubeNode);
    // FIXED: Call the curve editor function to display the curve UI!
    m_EditorUI.ShowCurveEditor(m_CurveState);

    m_EditorUI.EndMasterDockspace();
    m_EditorUI.EndFrame();
}

void Application::OnResize(uint32_t width, uint32_t height)
{
    m_Renderer.ResizeCanvas(width, height, k_EditorBg.r, k_EditorBg.g, k_EditorBg.b);
    m_DX11Presenter.Resize(width, height);
    m_DX11Presenter.Present(m_Renderer.GetFramebuffer(), !m_ResizePending);
}