find_package(GLUT REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/capture)
include_directories(${CMAKE_SOURCE_DIR}/libelas)
include_directories(${CMAKE_SOURCE_DIR}/libelas/src)
include_directories(${CMAKE_SOURCE_DIR}/libelasCV)

#add_executable(calib calib.cpp)
add_executable(retification retification.cpp)
add_executable(disparityCV disparityCV.cpp)
add_executable(elasDispCV disparityElasCv.cpp)

#target_link_libraries(calib videosource ${GLUT_LIBRARIES} cvd GL)
target_link_libraries(retification videosource ${OpenCV_LIBS} )
target_link_libraries(disparityCV videosource ${OpenCV_LIBS})
target_link_libraries(elasDispCV videosource ${OpenCV_LIBS} elasCV)
