cmake_minimum_required(VERSION "4.0.0")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(NE3DR LANGUAGES C CXX VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

message(${CMAKE_SOURCE_DIR})

add_executable(NE3DR 
    source/main.cpp
    source/viewer/shader.cpp
    dependancies/glad/glad.h
    dependancies/glad/glad.c
)


find_package(Vulkan)
target_link_libraries(NE3DR PRIVATE Vulkan::Vulkan)

target_link_libraries(NE3DR PRIVATE glfw)
target_link_libraries(NE3DR PRIVATE GL)
target_link_libraries(NE3DR PRIVATE glm)

set_property(TARGET NE3DR PROPERTY CXX_STANDARD 20)