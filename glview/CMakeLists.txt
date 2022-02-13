# CMake version
cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

# Declare project
project(ray-tracing VERSION 0.1.0)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GL_INCLUDE_DIRS})

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Build type - Release by default
# To build with debug, build with `cmake -DCMAKE_BUILD_TYPE=Debug ..`
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include libraries
add_subdirectory(raymath)

# Executables
file(GLOB SOURCES src/*.cpp)
add_executable(ray-tracing ${SOURCES})

target_include_directories(
  ${PROJECT_NAME} PUBLIC
  "${PROJECT_SOURCE_DIR}/raymath/include"
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
  glut OpenGL::GL
  glut OpenGL::GLU
  ${PROJECT_SOURCE_DIR}/raymath
)
