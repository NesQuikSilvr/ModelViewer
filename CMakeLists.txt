cmake_minimum_required(VERSION 3.16)

project(menv)

# Add source files
set(
    SOURCES
    main.cpp
    src/Camera.cpp
    src/Cube.cpp
    src/Shader.cpp
    src/Skeleton.cpp
    src/Tokenizer.cpp
    src/Window.cpp
    src/imgui/imgui.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_impl_glfw.cpp
    src/imgui/imgui_impl_opengl3.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
)

# Add header files
set(
    HEADERS
    include/core.h
    include/Camera.h
    include/Cube.h
    include/Shader.h
    include/Skeleton.h
    include/Tokenizer.h
    include/Window.h
)

# Require GL
find_package(OpenGL REQUIRED)

# Add include directories
include_directories(
    include
    include/imgui
)

# Add library directories
link_directories(
    lib
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} "include/Joint.h" "src/Joint.cpp" "include/Skin.h" "src/Skin.cpp" "include/Vertex.h" "src/Vertex.cpp" "include/Triangle.h" "src/Triangle.cpp" "include/Animation.h" "src/Animation.cpp" "include/Channel.h" "src/Channel.cpp" "include/Key.h" "src/Key.cpp")

# Link libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3)

# Move assets to .exe
add_custom_target(CopyShaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/shaders"
	"${CMAKE_BINARY_DIR}/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/skel"
	"${CMAKE_BINARY_DIR}/skel"
)
add_dependencies(menv CopyShaders)