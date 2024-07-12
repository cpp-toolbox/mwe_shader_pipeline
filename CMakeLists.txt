cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mwe_shader_pipeline)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(mwe_shader_pipeline
        src/main.cpp
        src/shader_pipeline/shader_pipeline.cpp
        src/window/window.cpp
)

find_package(glad)
find_package(glfw3)

target_link_libraries(${PROJECT_NAME} glad::glad glfw)