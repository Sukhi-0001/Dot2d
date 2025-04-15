cmake_minimum_required(VERSION 3.5.0)
set(NAME "dot2d")
project(${NAME})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#including libs
add_subdirectory(./libs)
add_executable(${NAME} ./src/main.cpp)
target_link_libraries(${NAME} PRIVATE SDL2::SDL2)


