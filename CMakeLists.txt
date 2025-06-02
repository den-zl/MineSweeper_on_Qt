cmake_minimum_required(VERSION 3.31)
project(MineSweeper_on_Qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_PREFIX_PATH "/Users/denzl/Qt/6.9.1/macos")

find_package(Qt6 6.9.1 REQUIRED COMPONENTS
        Core Gui Widgets StateMachine Concurrent Network
)

qt_standard_project_setup()

add_subdirectory(src)
