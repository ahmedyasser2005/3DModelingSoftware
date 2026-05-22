#include "Application.h"
#include "Draw/Draw.h"
Application::Application() : m_Window({ L"3D Modeling Software Engine", 1280, 720 })
{
    m_SoftwareRenderer.Initialize(m_Window.GetWidth(), m_Window.GetHeight());
    m_DX11Renderer.Initialize(m_Window.GetNativeHandle(), m_Window.GetWidth(), m_Window.GetHeight());

    m_SoftwareRenderer.Clear(30, 33, 40); // Sleek editor dark mode background

    m_Window.SetResizeCallback(
        [this](uint32_t w, uint32_t h)
        {
            m_SoftwareRenderer.ResizeCanvas(w, h, 30, 33, 40);
            m_DX11Renderer.Resize(w, h);

            m_DX11Renderer.Present(m_SoftwareRenderer.GetFramebuffer(), false);
        });
}

void Application::Run()
{
    while (m_IsRunning)
    {
        HandleEvents();
        if (!m_IsRunning)
            break;

        Update(0.016f);
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
    if (ww > 0 && wh > 0 && (ww != m_SoftwareRenderer.GetWidth() || wh != m_SoftwareRenderer.GetHeight()))
    {
        m_SoftwareRenderer.ResizeCanvas(ww, wh, 30, 33, 40);
        m_DX11Renderer.Resize(ww, wh);
    }
}

void Application::Update(float deltaTime)
{
    (void)deltaTime;

    const Input& input = m_Window.GetInput();

    if (input.IsKeyPressed(KeyCode::Escape))
    {
        m_IsRunning = false;
        return;
    }

    if (input.IsKeyPressed(KeyCode::Space))
    {
        m_SoftwareRenderer.Clear(30, 33, 40);
    }

    if (input.IsMouseButtonPressed(MouseButton::Left))
    {
        int32_t mx = input.GetMouseX();
        int32_t my = input.GetMouseY();

        // Draw gold pixels using a steady 2x2 brush layout
        m_SoftwareRenderer.PutPixel(mx, my, 255, 200, 0);
        m_SoftwareRenderer.PutPixel(mx + 1, my, 255, 200, 0);
        m_SoftwareRenderer.PutPixel(mx, my + 1, 255, 200, 0);
        m_SoftwareRenderer.PutPixel(mx + 1, my + 1, 255, 200, 0);
    }
    if (input.IsKeyPressed(KeyCode::K))
    {
        Draw::LineBresenham(&m_SoftwareRenderer, 100, 100, 400, 300, 0xFF00FF00);
    }
    if (input.IsKeyPressed(KeyCode::D)) 
            {
        Draw::LineDDA(&m_SoftwareRenderer, 200, 200, 500, 500, 0xFF0000FF);
            }
    if (input.IsKeyPressed(KeyCode::S))
         {
         Draw::CircleCartesian(&m_SoftwareRenderer, 400, 300, 100, 0xFFFF0000);
         }
    if (input.IsKeyPressed(KeyCode::C))
    {
        Draw::CirclePolar(&m_SoftwareRenderer, 600, 300, 80, 0xFFFFFF00);
    }
    if (input.IsKeyPressed(KeyCode::B))
     {
         Draw::CircleBresenham(&m_SoftwareRenderer, 800, 300, 60, 0xFFFF00FF);
     }
    if (input.IsKeyPressed(KeyCode::M))
    {
        Draw::MidpointCircle(&m_SoftwareRenderer, 1000, 300, 120, 0xFF00FFFF);
    }
    if (input.IsKeyPressed(KeyCode::I))
    {
        Draw::EllipseCartesian(&m_SoftwareRenderer, 300, 500, 120, 70, 0xFF00FF00);
    }
    if (input.IsKeyPressed(KeyCode::O))
    {
        Draw::EllipsePolar(&m_SoftwareRenderer, 640, 500, 120, 70, 0xFFFFFF00);
    }
    if (input.IsKeyPressed(KeyCode::P))
    {
        Draw::EllipseMidpoint(&m_SoftwareRenderer, 980, 500, 120, 70, 0xFF00FFFF);
    }
}

void Application::Render()
{
    m_DX11Renderer.Present(m_SoftwareRenderer.GetFramebuffer(), true);
}
