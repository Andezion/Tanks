cmake_minimum_required(VERSION 3.29)
project(Tanki)

set(CMAKE_CXX_STANDARD 20)

set(SFML_INCLUDE_DIR C://Library//SFML//SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit//SFML-2.6.2//include)
set(SFML_LIB_DIR C://Library//SFML//SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit//SFML-2.6.2//lib)

include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIB_DIR})

add_executable(Tanki main.cpp
        tank_types.h
        bullet_types.h
        maze.h
        supplies.h)

target_link_libraries(Tanki sfml-graphics sfml-window sfml-system sfml-audio)
