cmake_minimum_required(VERSION 3.25)
project(3DModelingSoftware VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /permissive-)
endif()



add_library(Platform STATIC 
    "src/Platform/Input.h"
    "src/Platform/Win32API.h"
    "src/Platform/Win32Window.h"
    "src/Platform/Win32Window.cpp"
)
target_include_directories(Platform PUBLIC "src/")



add_executable(${PROJECT_NAME} WIN32
    "src/Main.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "src/")
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Platform 
)