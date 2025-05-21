cmake_minimum_required(VERSION 3.31)
project(MineSweeper_on_Qt)

set(CMAKE_CXX_STANDARD 20)

add_executable(MineSweeper_on_Qt
        src/main.cpp)

add_subdirectory(src)
