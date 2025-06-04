#[[set(PROJECT_FILES
  "./src/game.cpp"
  "./src/ecs/ecs.cpp"
  "./src/main.cpp"
  "./src/assets_manager.cpp"
)
cmake_minimum_required(VERSION 3.5.0)
set(NAME "dot2d")
project(${NAME})
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
#including libs
add_subdirectory(./libs)
add_executable(${NAME} ${PROJECT_FILES})
#including folders
#include_directories(include/)
#target_include_directories(${NAME} PRIVATE ./include/)
target_include_directories(${NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${NAME} PRIVATE SDL2_image::SDL2_image)
target_link_libraries(${NAME} PRIVATE glm::glm)
target_link_libraries(${NAME} PRIVATE spdlog::spdlog)
]]


# Minimum required CMake version
cmake_minimum_required(VERSION 3.5.0)

# Define project name
set(NAME "dot2d")
project(${NAME})
message(STATUS "Include directory is: ${CMAKE_SOURCE_DIR}/include")
# Set build type to Debug
set(CMAKE_BUILD_TYPE Debug)

# Debug flags (optional)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Enable exporting compile commands (useful for IDEs like VSCode)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ Standard version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

# List of source files for your project
set(PROJECT_FILES
    "./src/game.cpp"
    "./src/ecs/ecs.cpp"
    "./src/main.cpp"
    "./src/assets_manager.cpp"
)

# Add external libraries (assuming SDL2 is built within ./libs)
add_subdirectory(./libs)

# Create executable from project files
add_executable(${NAME} ${PROJECT_FILES})

# Include directories (must be set correctly)
target_include_directories(${NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/include
)

# Link required libraries
target_link_libraries(${NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${NAME} PRIVATE SDL2_image::SDL2_image)
target_link_libraries(${NAME} PRIVATE SDL2::SDL2 SDL2_ttf)
target_link_libraries(${NAME} PRIVATE glm::glm)
target_link_libraries(${NAME} PRIVATE spdlog::spdlog)
target_link_libraries(${NAME} PRIVATE imgui)


