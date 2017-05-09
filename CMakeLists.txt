#project
cmake_minimum_required(VERSION 2.8.7)
project(VisualOdometry-Gus)

find_package(OpenCV REQUIRED)
include_directories($(OpenCV_INCLUDE_DIRS))

#definitons
add_definitions(-pthread -std=c++11)

# directories
set (VO_SRC_DIR src)
set (LIBELAS_DIR libelas)
set (LIBELASCV_DIR libelasCV)
set (CAPTURE_DIR capture)
set (CALIB_DIR calib)

#include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${VO_SRC_DIR})
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LIBELAS_DIR})
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LIBELASCV_DIR})
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${CAPTURE_DIR})
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${CALIB_DIR})

#output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib) 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib) 

