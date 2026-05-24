#include "EditorUI.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <stdexcept>

EditorUI::EditorUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
}

EditorUI::~EditorUI()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void EditorUI::Initialize(void* hwnd, void* device, void* context)
{
    if (!hwnd || !device || !context)
        throw std::runtime_error("No hwnd or device or context provided to ImGui!");

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(reinterpret_cast<ID3D11Device*>(device), reinterpret_cast<ID3D11DeviceContext*>(context));
}

void EditorUI::StartFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void EditorUI::EndFrame()
{
    ImGui::EndFrame();
    ImGui::Render();
}

// Example function of EditorUI function
// void EditorUI::CreateMainMenu()
// {
//     ImGui::Begin("Hello, world!");
//     ImGui::Text("This is some useful text.");
//     if (ImGui::Button("Button"))
//     {
//          ////////// DON'T IMPLEMENT IT, JUST LEAVE A COMMENT OF INTENTION HERE! //////////
//     }
//     ImGui::End();
// }