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

# Setting up an opengl window. Do not put this in the ray tracing
#   source though. Viewing the ray tracer should be a separate project.
#   Whether it be WebGL, Glui, or GTX https://www.gtk.org/.
# set(OpenGL_GL_PREFERENCE GLVND)
# find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)

# include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

# target_link_libraries(ray-tracing ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
