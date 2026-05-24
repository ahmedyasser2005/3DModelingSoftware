#pragma once

class Renderer;
struct Node;
struct CurveData;


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
    void ShowCurveEditor(CurveData& curve);

    void ShowEditorPanels(Renderer& renderer, Node* targetNode);
  
};