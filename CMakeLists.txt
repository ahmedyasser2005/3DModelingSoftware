cmake_minimum_required(VERSION 3.25)
project(3DModelingSoftware VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
endif()

add_executable(${PROJECT_NAME} WIN32
    "src/Main.cpp"

    "src/Platform/Win32API.h"
    "src/Platform/Window.h"
    "src/Platform/Window.cpp"
    "src/Platform/Input.h"

    "src/Renderer/DX11Renderer.h"
    "src/Renderer/DX11Renderer.cpp"
    "src/Renderer/SoftwareRenderer.h"
    "src/Renderer/SoftwareRenderer.cpp"

    "src/Draw/Draw.h"
    "src/Draw/Draw.cpp"

    "src/Application/Application.h"
    "src/Application/Application.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC d3d11 dxgi d3dcompiler)
endif()