# use sse3 instruction set
SET(CMAKE_CXX_FLAGS "-msse3" )

# sources
FILE(GLOB LIBELASCV_SRC_FILES "*.cpp")

# make release version
set(CMAKE_BUILD_TYPE Release)

# build demo program
add_library(elasCV ${LIBELASCV_SRC_FILES})

