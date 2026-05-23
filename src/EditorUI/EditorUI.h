#pragma once

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
};