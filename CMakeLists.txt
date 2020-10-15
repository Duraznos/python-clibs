cmake_minimum_required(VERSION 3.15)
project(python_libs C)

set(CMAKE_C_STANDARD 99)

add_library(python_libs SHARED library.c library.h)
find_package(Python3 COMPONENTS Development)
include_directories(${Python3_INCLUDE_DIRS})
target_link_libraries(python_libs ${Python3_LIBRARIES})

