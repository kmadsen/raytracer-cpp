# Declare project
project(ray-tracing)

# CMake version
cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

# OpenGL_GL_PREFERENCE has not been set to "GLVND" or "LEGACY"
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
# set(DEPENDENCIES ray-tracing glfw ${OPENGL_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL )

include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GL_INCLUDE_DIRS})

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

target_link_libraries(${PROJECT_NAME}
    glut OpenGL::GL
    glut OpenGL::GLU
)
