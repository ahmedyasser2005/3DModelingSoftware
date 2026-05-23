#pragma once

#include "Platform/Window.h"
#include "Renderer/DX11Renderer.h"
#include "Renderer/SoftwareRenderer.h"

class Application final
{
  public:
    Application();
    ~Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void Run();

  private:
    void HandleEvents();
    void Update(const float& dt);
    void Render();

    void OnResize(uint32_t w, uint32_t h);

  private:
    Window m_Window;
    SoftwareRenderer m_SoftwareRenderer;
    DX11Renderer m_DX11Renderer;

    bool m_IsRunning = true;
    bool m_ResizePending = false;
};
