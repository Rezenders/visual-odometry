include_directories(${CMAKE_SOURCE_DIR}/capture)

add_executable(test test.cpp)
target_link_libraries(test videosource)

add_executable(stereobm testStereoBM.cpp)
target_link_libraries(stereobm videosource)
