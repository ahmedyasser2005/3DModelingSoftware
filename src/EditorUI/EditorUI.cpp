#include "EditorUI.h"
#include "Draw/Draw.h"         // Matches your folder hierarchy
#include "Renderer/Renderer.h" // For your Renderer reference
#include <algorithm>           // Fixes std::abs and std::max
#include <cmath>               // Fixes std::sqrt and std::round
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
    ImGui::EndFrame();
    ImGui::Render();
}

void EditorUI::BeginMasterDockspace()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
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
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
}

void EditorUI::EndMasterDockspace()
{
    ImGui::End();
}

void EditorUI::ShowEditorPanels(Renderer& renderer)
{
    ImGui::Begin("Toolbox Panel");
    {
        // ==========================================
        // SECTION 1: GLOBAL COLOR OPTIONS
        // ==========================================
        static ImVec4 selected_color = ImVec4(1.0f, 0.78f, 0.0f, 1.0f); // Default amber
        ImGui::Text("Primitive Options");
        ImGui::ColorEdit4("Draw Color", &selected_color.x, ImGuiColorEditFlags_NoAlpha);

        uint32_t colorHex = ((static_cast<uint32_t>(selected_color.x * 255.0f)) << 16) |
                            ((static_cast<uint32_t>(selected_color.y * 255.0f)) << 8) |
                            (static_cast<uint32_t>(selected_color.z * 255.0f));

        ImGui::Spacing();
        ImGui::Separator();

        // ==========================================
        // SECTION 2: COORDINATE INPUT FIELDS
        // ==========================================
        ImGui::Text("Coordinate Entry Workspace");

        // Static variables hold onto your typed values across frames
        static int32_t x0 = 100, y0 = 100;
        static int32_t x1 = 300, y1 = 300;
        static int32_t radius = 50;
        static int32_t rx = 80, ry = 40;

        // Visual layout groupings for organized input fields
        ImGui::Text("Point A / Center (X, Y):");
        ImGui::PushItemWidth(120);
        ImGui::InputInt("Start X", &x0);
        ImGui::SameLine();
        ImGui::InputInt("Start Y", &y0);
        ImGui::PopItemWidth();

        ImGui::Text("Point B (Lines Only):");
        ImGui::PushItemWidth(120);
        ImGui::InputInt("End X", &x1);
        ImGui::SameLine();
        ImGui::InputInt("End Y", &y1);
        ImGui::PopItemWidth();

        ImGui::Text("Dimensions (Shapes Only):");
        ImGui::PushItemWidth(120);
        ImGui::InputInt("Circle Radius", &radius);
        ImGui::InputInt("Ellipse Rx", &rx);
        ImGui::SameLine();
        ImGui::InputInt("Ellipse Ry", &ry);
        ImGui::PopItemWidth();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Execute Commands:");
        ImGui::Separator();

        // ==========================================
        // SECTION 3: LINE COMMAND BUTTONS
        // ==========================================
        if (ImGui::TreeNode("Line Utilities"))
        {
            if (ImGui::Button("Execute Bresenham Line"))
            {
                Draw::LineBresenham(renderer, x0, y0, x1, y1, colorHex);
            }
            if (ImGui::Button("Execute Fractional Line"))
            {
                Draw::LineFractionalAccumulation(renderer, x0, y0, x1, y1, colorHex);
            }
            if (ImGui::Button("Execute DDA Line"))
            {
                Draw::LineDDA(renderer, x0, y0, x1, y1, colorHex);
            }
            ImGui::TreePop();
        }

        // ==========================================
        // SECTION 4: CIRCLE COMMAND BUTTONS
        // ==========================================
        if (ImGui::TreeNode("Circle Utilities"))
        {
            if (radius > 0)
            {
                if (ImGui::Button("Execute Bresenham Circle"))
                {
                    Draw::CircleBresenham(renderer, x0, y0, radius, colorHex);
                }
                if (ImGui::Button("Execute Midpoint Circle"))
                {
                    Draw::CircleMidpoint(renderer, x0, y0, radius, colorHex);
                }
                if (ImGui::Button("Execute Polar Circle"))
                {
                    Draw::CirclePolar(renderer, x0, y0, radius, colorHex);
                }
                if (ImGui::Button("Execute Cartesian Circle"))
                {
                    Draw::CircleCartesian(renderer, x0, y0, radius, colorHex);
                }
            }
            else
            {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Set Radius > 0 to render!");
            }
            ImGui::TreePop();
        }

        // ==========================================
        // SECTION 5: ELLIPSE COMMAND BUTTONS
        // ==========================================
        if (ImGui::TreeNode("Ellipse Utilities"))
        {
            if (rx > 0 && ry > 0)
            {
                if (ImGui::Button("Execute Midpoint Ellipse"))
                {
                    Draw::EllipseMidpoint(renderer, x0, y0, rx, ry, colorHex);
                }
                if (ImGui::Button("Execute Fractional Ellipse"))
                {
                    Draw::EllipseFractionalAccumulation(renderer, x0, y0, rx, ry, colorHex);
                }
                if (ImGui::Button("Execute Polar Ellipse"))
                {
                    Draw::EllipsePolar(renderer, x0, y0, rx, ry, colorHex);
                }
                if (ImGui::Button("Execute Cartesian Ellipse"))
                {
                    Draw::EllipseCartesian(renderer, x0, y0, rx, ry, colorHex);
                }
            }
            else
            {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Set Rx and Ry > 0 to render!");
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}