cmake_minimum_required(VERSION 3.15)
project(crypto_test)
find_package(Threads)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
add_definitions(/experimental:concepts)
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
else()
add_definitions(-fconcepts -ffast-math)
endif()

set(CMAKE_CXX_STANDARD 20)


add_executable(ctr "ctr.cc")
add_executable(ctr_bin "ctr_binary_serialization.cc")