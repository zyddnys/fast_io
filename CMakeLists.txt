cmake_minimum_required(VERSION 3.15)
project(fastio)
find_package(Threads)

INSTALL (DIRECTORY ${CMAKE_SOURCE_DIR}/include/
DESTINATION ${PROJECT_SOURCE_DIR}/include)
