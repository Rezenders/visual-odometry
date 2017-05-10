add_library(retification retification.cpp)
target_link_libraries(retification ${OpenCV_LIBS})

add_library(videosource VideoSource.cpp)
target_link_libraries(videosource retification ${OpenCV_LIBS})
