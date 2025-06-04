add_subdirectory(./SDL)
add_subdirectory(./glm)
add_subdirectory(./spdlog)
add_subdirectory(./SDL_image)
add_subdirectory(./SDL_ttf)
# make imgui with sdl 2

cmake_minimum_required(VERSION 3.10)
project(imgui)

# ImGui source files
set(IMGUI_SRC
    ./imgui/imgui.cpp
    ./imgui/imgui_draw.cpp
    ./imgui/imgui_tables.cpp
    ./imgui/imgui_widgets.cpp
    ./imgui/imgui_demo.cpp
    ./imgui/backends/imgui_impl_sdl2.cpp
    ./imgui/backends/imgui_impl_sdlrenderer2.cpp
)

# Create static ImGui library
add_library(imgui STATIC ${IMGUI_SRC})

# Public include directories for consumers
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/SDL/include
)
