cmake_minimum_required(VERSION 2.8.7)
project(visual-odometry)

find_package(OpenCV REQUIRED)

add_definitions(-Wall -g -std=c++11)

#include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tools)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/calib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

#output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)
