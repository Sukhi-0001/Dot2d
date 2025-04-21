set(PROJECT_FILES
  "./src/main.cpp"
  "./src/game.cpp"
  "./src/ecs/ecs.cpp"
)
cmake_minimum_required(VERSION 3.5.0)
set(NAME "dot2d")
project(${NAME})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
#including libs
add_subdirectory(./libs)
add_executable(${NAME} ${PROJECT_FILES})
#including folders
target_include_directories(${NAME} PRIVATE ./inlcude/)
target_link_libraries(${NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${NAME} PRIVATE SDL2_image::SDL2_image)
target_link_libraries(${NAME} PRIVATE glm::glm)
target_link_libraries(${NAME} PRIVATE spdlog::spdlog)


