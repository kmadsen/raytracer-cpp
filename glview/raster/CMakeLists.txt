# CMake version
cmake_minimum_required(VERSION 3.15)

# Declare project
project(raster VERSION 0.1.0)

option(WITH_TEST "Build with GoogleTest" OFF)

# Create the library
file(GLOB SOURCES src/*.cpp)
add_library(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

include_directories(
  ${OPENGL_INCLUDE_DIR}
  ${GL_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
add_subdirectory(../tinygltf build)

# Link private libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
  ${OPENGL_gl_LIBRARY}
  glfw ${GLFW_LIBRARIES}
  GLEW ${GLEW_LIBRARIES}
  tinygltf
)

# Use GoogleTest
#   cd build && cmake .. -DWITH_TEST=ON && make && ctest
if(WITH_TEST)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  enable_testing()

  file(GLOB SOURCES_TEST test/*.cpp)
  add_executable(raster_test ${SOURCES_TEST})
  target_link_libraries(raster_test ${PROJECT_NAME} gtest_main)

  include(GoogleTest)
  gtest_discover_tests(raster_test)
endif()
