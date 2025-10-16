cmake_minimum_required(VERSION 3.28)
project(vector_calculator)

set(CMAKE_CXX_STANDARD 20)

# Find SFML 3.0
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

add_executable(vector_calculator main.cpp
        models/Vector2D.cpp
        models/Vector2D.h
        models/VectorSpace.h)

# Link SFML 3 libraries with the SFML:: namespace
target_link_libraries(vector_calculator SFML::Graphics SFML::Window SFML::System)