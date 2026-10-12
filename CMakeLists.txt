cmake_minimum_required(VERSION 3.10)
project(OOPVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2TTF REQUIRED SDL2_ttf)

add_executable(oop_visualizer
    src/main.cpp
    src/Simulator.cpp
    src/Renderer.cpp
)

target_include_directories(oop_visualizer PRIVATE
    include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2TTF_INCLUDE_DIRS}
)

target_link_libraries(oop_visualizer PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2TTF_LIBRARIES}
)

target_compile_options(oop_visualizer PRIVATE
    ${SDL2_CFLAGS_OTHER}
    ${SDL2TTF_CFLAGS_OTHER}
)
