cmake_minimum_required(VERSION 3.15)
project(binary_size)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
add_definitions(/experimental:concepts)
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
else()
add_definitions(-fconcepts -ffast-math)
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(stdio_hello_world "./stdio_hello_world.c")
add_executable(iostream_hello_world "./iostream_hello_world.cc")
add_executable(fast_io_hello_world "./fast_io_hello_world.cc")

SET(CMAKE_EXE_LINKER_FLAGS "-s")