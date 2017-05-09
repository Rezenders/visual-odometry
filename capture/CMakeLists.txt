find_package(libfreenect REQUIRED)
find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/libelas/src)

add_library(videosource VideoSource.cpp)
add_library(format_conversion format_conversion.cpp)

target_link_libraries(videosource format_conversion ${OpenCV_LIBS}  cvd)
