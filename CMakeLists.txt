cmake_minimum_required(VERSION 3.25)
project(3DModelingSoftware VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    "src/Main.cpp"

    "src/Core/Window.h"
    "src/Core/Window.cpp"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME} PUBLIC d3d11 dxgi d3dcompiler)