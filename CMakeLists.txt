cmake_minimum_required(VERSION 3.12)
project(Playter)

set(CMAKE_CXX_STANDARD 17)

# Dependencies Setup
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG master
)
FetchContent_MakeAvailable(glfw)

# ImGui Paths
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/vendor/imgui")
file(GLOB IMGUI_SOURCES
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/*.h"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

# Miniaudio
set(MINIAUDIO_DIR "${CMAKE_SOURCE_DIR}/vendor/miniaudio")

# Project Includes
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${MINIAUDIO_DIR}
    ${glfw_SOURCE_DIR}/include
)

# Define Executable
add_executable(Playter
    src/main.cpp
    rc/Playter.cpp 
    ${IMGUI_SOURCES}
)

# Linking
find_package(OpenGL REQUIRED)
target_link_libraries(Playter PRIVATE glfw OpenGL::GL)

if(UNIX AND NOT APPLE)
    target_link_libraries(Playter PRIVATE pthread dl)
elseif(APPLE)
    target_link_frameworks(Playter PRIVATE CoreFoundation CoreAudio AudioToolbox)
endif()