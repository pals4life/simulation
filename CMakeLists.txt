cmake_minimum_required(VERSION 3.6)
project(simulation)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

file(GLOB_RECURSE HDRS ${PSE_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE SRCS ${PSE_SOURCE_DIR}/src/*.cpp)

# Set source files for RELEASE target
<<<<<<< Updated upstream
set(RELEASE_SOURCE_FILES ${SRCS} ${HDRS})

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES ${SRCS} ${HDRS})
=======
set(RELEASE_SOURCE_FILES NetworkParser.cpp)

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES NetworkParser.cpp)
>>>>>>> Stashed changes

# Create RELEASE target
add_executable(simulation ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(simulation_debug ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(simulation_debug gtest)
