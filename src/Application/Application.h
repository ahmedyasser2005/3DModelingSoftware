#pragma once

#include "Platform/Win32API.h"

#include "Camera/Camera.h"
#include "EditorUI/EditorUI.h"
#include "Platform/Window.h"
#include "Presenter/DX11Presenter.h"
#include "Renderer/Renderer.h"
#include "Scene/SceneGraph.h"

class Application final
{
  public:
    Application();
    ~Application()                             = default;
    Application(const Application&)            = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&)                 = delete;
    Application& operator=(Application&&)      = delete;

    void Run();

  private:
    void        HandleEvents();
    inline void FirstFrame();
    void        Update(const float& deltaTime);
    void        Render();
    void        RenderUI();

    void OnResize(uint32_t width, uint32_t height);

  private:
    Window        m_Window;
    Renderer      m_Renderer;
    SceneGraph    m_SceneGraph;
    DX11Presenter m_DX11Presenter;
    EditorUI      m_EditorUI;
    Camera        m_Camera;

    bool m_IsRunning     = true;
    bool m_ResizePending = false;
};
