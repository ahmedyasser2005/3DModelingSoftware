#pragma once

// Keep track of what we want to draw if we use states
enum class DrawingTool
{
    None,
    BresenhamLine,
    FractionalLine,
    DDALine,
    BresenhamCircle,
    MidpointCircle,
    PolarCircle,
    CartesianCircle,
    MidpointEllipse,
    FractionalEllipse,
    PolarEllipse,
    CartesianEllipse
};

class EditorUI final
{
  public:
    EditorUI();
    ~EditorUI();
    EditorUI(const EditorUI&) = delete;
    EditorUI& operator=(const EditorUI&) = delete;
    EditorUI(EditorUI&&) = delete;
    EditorUI& operator=(EditorUI&&) = delete;

    void Initialize(void* hwnd, void* device, void* context);
    void StartFrame();
    void EndFrame();

    void BeginMasterDockspace();
    void EndMasterDockspace();

    // Updated declaration to accept your software renderer
    void ShowEditorPanels(class Renderer& renderer);

  private:
    DrawingTool m_ActiveTool = DrawingTool::None;
};