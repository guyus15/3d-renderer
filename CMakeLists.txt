cmake_minimum_required(VERSION 3.22)
project(3d_renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./out)

# glfw
add_subdirectory(libs/glfw)

include_directories(include/
                    libs/glad/include
                    libs/glfw/include
                    libs/glm
                    libs/imgui
                    libs/imgui/backends)

set(SOURCE_FILES src/main.cpp
                 src/shader.cpp
                 src/texture.cpp
                 src/resource_manager.cpp
                 src/camera.cpp
                 libs/glad/src/glad.c
                 libs/imgui/imgui.cpp
                 libs/imgui/imgui_demo.cpp
                 libs/imgui/imgui_draw.cpp
                 libs/imgui/imgui_tables.cpp
                 libs/imgui/imgui_widgets.cpp
                 libs/imgui/backends/imgui_impl_glfw.cpp
                 libs/imgui/backends/imgui_impl_opengl3.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
        COMMENT "Copying resources into build directory")

add_dependencies(${PROJECT_NAME} copy_resources)