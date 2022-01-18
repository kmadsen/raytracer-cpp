# Declare project
project(ray-tracing)

# CMake version
cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Build type - Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Executables
file(GLOB SOURCES src/*.cpp)
add_executable(ray-tracing ${SOURCES})
