cmake_minimum_required(VERSION 3.15)
project(fastio)
find_package(Threads)

INSTALL (DIRECTORY ${PROJECT_SOURCE_DIR}/include/
DESTINATION include
FILES_MATCHING PATTERN "*.h")
