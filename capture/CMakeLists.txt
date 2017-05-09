find_package(libfreenect REQUIRED)
find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/libelas/src)

add_library(videosource VideoSource.cpp)

target_link_libraries(videosource ${OpenCV_LIBS} )
