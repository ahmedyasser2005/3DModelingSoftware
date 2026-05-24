#include "EditorUI.h"
#include "Application/Application.h"
#include "Common/CurveData.h"
#include "Renderer/Renderer.h"
#include "Scene/Node.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <stdexcept>

EditorUI::EditorUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void EditorUI::BeginMasterDockspace()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |=
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("MainDockingShellSystem", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("CentralAppDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
}

void EditorUI::EndMasterDockspace()
{
    ImGui::End();
}

void EditorUI::ShowEditorPanels(Renderer& renderer, Node* targetNode)
{
    (void)renderer;

    ImGui::SetNextWindowSize(ImVec2(300, -1));

    ImGui::Begin("Toolbox Panel");
    {
        if (targetNode != nullptr)
        {
            ImGui::Text("Active Object: %s", targetNode->Name.c_str());
            ImGui::Separator();

            ImGui::Text("Position Modification:");
            // FIXED: Using targetNode, and properly splitting x, y, and z
            ImGui::DragFloat("X", &targetNode->Position.x, 0.1f);
            ImGui::DragFloat("Y", &targetNode->Position.y, 0.1f);
            ImGui::DragFloat("Z", &targetNode->Position.z, 0.1f);

            ImGui::Separator();
            ImGui::Text("Rotation Modification:");
            ImGui::DragFloat("Pitch", &targetNode->Rotation.x, 0.05f);
            ImGui::DragFloat("Yaw", &targetNode->Rotation.y, 0.05f);
            ImGui::DragFloat("Roll", &targetNode->Rotation.z, 0.05f);

            ImGui::Separator();
            ImGui::Text("Scale Modification:");
            ImGui::DragFloat("Scale X", &targetNode->Scale.x, 0.05f);
            ImGui::DragFloat("Scale Y", &targetNode->Scale.y, 0.05f);
            ImGui::DragFloat("Scale Z", &targetNode->Scale.z, 0.05f);
        }
        else
        {
            ImGui::Text("No active 3D nodes found in scene.");
        }
    }
    ImGui::End();
}

void EditorUI::ShowCurveEditor(CurveData& curve)
{
    ImGui::Begin("Curve Toolbox");

    ImGui::Checkbox("Show Curve", &curve.IsVisible);

    const char* curveTypes[] = { "Cubic Bezier", "Rational Bezier", "B-Spline" };
    int currentType = static_cast<int>(curve.Type);
    if (ImGui::Combo("Type", &currentType, curveTypes, IM_ARRAYSIZE(curveTypes)))
    {
        curve.Type = static_cast<CurveType>(currentType);
    }

    ImGui::SeparatorText("Properties");
    ImGui::ColorEdit3("Color", curve.Color);
    ImGui::SliderInt("Segments", &curve.Segments, 4, 128);

    ImGui::SeparatorText("Control Points");
    float dragSpeed = 0.05f;
    ImGui::DragFloat3("P0", &curve.P0.x, dragSpeed);
    ImGui::DragFloat3("P1", &curve.P1.x, dragSpeed);
    ImGui::DragFloat3("P2", &curve.P2.x, dragSpeed);
    ImGui::DragFloat3("P3", &curve.P3.x, dragSpeed);

    if (curve.Type == CurveType::RationalBezier)
    {
        ImGui::SeparatorText("Weights");
        ImGui::DragFloat("W0", &curve.W0, dragSpeed);
        ImGui::DragFloat("W1", &curve.W1, dragSpeed);
        ImGui::DragFloat("W2", &curve.W2, dragSpeed);
        ImGui::DragFloat("W3", &curve.W3, dragSpeed);
    }

    ImGui::End();
}