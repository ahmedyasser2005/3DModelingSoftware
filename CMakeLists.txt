cmake_minimum_required(VERSION 3.25)
project(3DModelingSoftware VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    "src/Main.cpp"

    "src/Platform/Input.h"
    "src/Platform/Win32API.h"
    "src/Platform/Win32Window.h"
    "src/Platform/Win32Window.cpp"
    
    "src/Renderer/DX11Renderer.h"
    "src/Renderer/DX11Renderer.cpp"
    
    "src/Renderer/SoftwareRenderer.h"
    "src/Renderer/SoftwareRenderer.cpp"

    "src/Application/Application.h"
    "src/Application/Application.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC "src/")

target_link_libraries(${PROJECT_NAME} PUBLIC d3d11 dxgi d3dcompiler)