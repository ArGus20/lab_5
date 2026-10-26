cmake_minimum_required(VERSION 3.20)
project(SFML_Lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-3.1.0/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(SFML_Lab main.cpp)

target_link_libraries(SFML_Lab PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)