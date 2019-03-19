cmake_minimum_required(VERSION 3.6)
project(simulation)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(${simulation_SOURCE_DIR}/gtest/include)

# Set Library dir
link_directories(${simulation_SOURCE_DIR}/gtest/lib)

# Set SOURCE and HEADER files
file(GLOB_RECURSE HDRS ${simulation_SOURCE_DIR}/src/datatypes/*.h   ${simulation_SOURCE_DIR}/src/parsers/*.h   ${simulation_SOURCE_DIR}/src/tests/*.h  )
file(GLOB_RECURSE SRCS ${simulation_SOURCE_DIR}/src/datatypes/*.cpp ${simulation_SOURCE_DIR}/src/parsers/*.cpp ${simulation_SOURCE_DIR}/src/tests/*.cpp)

# Set source files for RELEASE/DEBUG target
set(RELEASE_SOURCE_FILES ${SRCS} ${HDRS} src/main.cpp    )
set(DEBUG_SOURCE_FILES   ${SRCS} ${HDRS} src/testMain.cpp)

# Create RELEASE / DEBUG target
add_executable(simulation ${RELEASE_SOURCE_FILES})
add_executable(simulation_debug ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(simulation_debug gtest)

# Link library
target_link_libraries(simulation gtest)
