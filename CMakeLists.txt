cmake_minimum_required(VERSION 3.25)
project(3DModelingSoftware VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
endif()

add_library(imgui STATIC
    "ext/imgui/imgui.cpp"
    "ext/imgui/imgui_demo.cpp"
    "ext/imgui/imgui_draw.cpp"
    "ext/imgui/imgui_tables.cpp"
    "ext/imgui/imgui_widgets.cpp"
    "ext/imgui/backends/imgui_impl_dx11.cpp"
    "ext/imgui/backends/imgui_impl_win32.cpp"
)
target_include_directories(imgui PUBLIC "ext/imgui" "ext/imgui/backends")

add_library(draw STATIC
    "src/Draw/Line.cpp"
    "src/Draw/Circle.cpp"
    "src/Draw/Ellipse.cpp"
)
target_include_directories(draw PUBLIC "src/")

add_executable(${PROJECT_NAME} WIN32
    "src/Main.cpp"

    "src/Platform/Win32API.h"
    "src/Platform/Window.h"
    "src/Platform/Window.cpp"
    "src/Platform/InputHandler.h"

    "src/Presenter/DX11Presenter.h"
    "src/Presenter/DX11Presenter.cpp"

    "src/Renderer/Mesh.h"
    "src/Renderer/Renderer.h"
    "src/Renderer/Renderer.cpp"
    "src/Renderer/WireframeRasterizer.h"
    "src/Renderer/WireframeRasterizer.cpp"

    "src/Application/Application.h"
    "src/Application/Application.cpp"

    "src/EditorUI/EditorUI.h"
    "src/EditorUI/EditorUI.cpp"

    "src/Scene/SceneGraph.h"
    "src/Scene/SceneGraph.cpp"

    "src/Camera/Camera.h"
    "src/Camera/Camera.cpp"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC d3d11 dxgi d3dcompiler PRIVATE imgui draw)
endif()