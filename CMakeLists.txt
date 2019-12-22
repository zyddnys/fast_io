cmake_minimum_required(VERSION 3.15)
project(fastio)
find_package(Threads)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
add_definitions(/experimental:concepts)
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
message(FATAL_ERROR "fast_io no longer supports GCC < 10 version because of incomplete C++ standard library support for concepts lite. Please build the master version of GCC from its official or mirror repo. Or use Visual Studio Preview")
endif()
add_definitions(-s -ffast-math)
add_link_options("-s")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
add_definitions(-Xclang -fconcepts-ts -ffast-math -s)
add_link_options("-s")
endif()
set(CMAKE_CXX_STANDARD 20)

add_library(cxx_fast_io_c_interface SHARED "./src/fast_io_c_interface.cc")
set_target_properties(cxx_fast_io_c_interface PROPERTIES SOVERSION 1)

INSTALL (DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
