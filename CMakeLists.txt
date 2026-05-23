cmake_minimum_required(VERSION 3.25)
project(3DModelingSoftware VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(
        /W4
        /WX
        /permissive-
    )
endif()

add_library(Platform STATIC 
    "src/Platform/Win32API.h"
    "src/Platform/Window.h"
    "src/Platform/Window.cpp"
    "src/Platform/Input.h"
    
)
target_include_directories(Platform PUBLIC "src/")



add_library(HardwareRenderer STATIC
    "src/Renderer/DX11Renderer.h"
    "src/Renderer/DX11Renderer.cpp"
)
target_include_directories(HardwareRenderer PUBLIC "src/")

if(WIN32)
    target_link_libraries(HardwareRenderer PUBLIC 
        d3d11.lib 
        dxgi.lib 
        d3dcompiler.lib
    )
endif()



add_library(SoftwareRenderer STATIC
    "src/Renderer/SoftwareRenderer.h"
    "src/Renderer/SoftwareRenderer.cpp"
)
target_include_directories(SoftwareRenderer PUBLIC "src/")



add_library(App STATIC
    "src/Application/Application.h"
    "src/Application/Application.cpp"
)
target_include_directories(App PUBLIC "src/")



add_executable(${PROJECT_NAME} WIN32
    "src/Main.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "src/")
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Platform 
    HardwareRenderer 
    SoftwareRenderer 
    App
)