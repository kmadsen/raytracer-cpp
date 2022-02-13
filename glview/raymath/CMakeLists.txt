# CMake version
cmake_minimum_required(VERSION 3.15)

# Declare project
project(raymath VERSION 0.1.0)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create the library
file(GLOB SOURCES src/*.cpp)
add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

option(WITH_TEST "BUILD THE TEST CODE" OFF)

if(WITH_TEST)

  # Use GoogleTest - run with `cd build && make && ctest`
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
  add_executable(raymath_test ${SOURCES_TEST})
  target_link_libraries(raymath_test ${PROJECT_NAME} gtest_main)

  include(GoogleTest)
  gtest_discover_tests(raymath_test)

endif()
