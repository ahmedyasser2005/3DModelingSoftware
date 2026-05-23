#pragma once

#include "Platform/Window.h"
#include "Presenter/DX11Presenter.h"
#include "Renderer/Renderer.h"

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
    Renderer m_Renderer;
    DX11Presenter m_DX11Presenter;

    bool m_IsRunning = true;
    bool m_ResizePending = false;
};
