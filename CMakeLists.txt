cmake_minimum_required(VERSION 3.6)
project(simulation)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-std=c++98 -Wall -Werror -Wnon-virtual-dtor -Wcast-align -Wunused -Wpedantic -Wduplicated-cond -Wlogical-op")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

# Set include dir
include_directories(${simulation_SOURCE_DIR}/gtest/include)

# Set Library dir
link_directories(${simulation_SOURCE_DIR}/gtest/lib)

file(GLOB_RECURSE HDRS ${simulation_SOURCE_DIR}/src/datatypes/*.h   ${simulation_SOURCE_DIR}/src/parsers/*.h   ${simulation_SOURCE_DIR}/src/gui/*.h   ${simulation_SOURCE_DIR}/src/exporters/*.h)
file(GLOB_RECURSE SRCS ${simulation_SOURCE_DIR}/src/datatypes/*.cpp ${simulation_SOURCE_DIR}/src/parsers/*.cpp ${simulation_SOURCE_DIR}/src/gui/*.cpp ${simulation_SOURCE_DIR}/src/exporters/*.cpp)

file(GLOB_RECURSE DEBUG_HDRS ${simulation_SOURCE_DIR}/src/tests/*.h  )
file(GLOB_RECURSE DEBUG_SRCS ${simulation_SOURCE_DIR}/src/tests/*.cpp )

# Set source files for RELEASE/DEBUG target
set(RELEASE_SOURCE_FILES ${SRCS} ${HDRS} src/main.cpp)
set(DEBUG_SOURCE_FILES   ${SRCS} ${HDRS} ${DEBUG_SRCS} ${DEBUG_HDRS} src/testMain.cpp)

# Create RELEASE / DEBUG target
add_executable(simulation       ${RELEASE_SOURCE_FILES})
add_executable(simulation_debug ${DEBUG_SOURCE_FILES}  )

# Link library
target_link_libraries(simulation_debug gtest)

# Link library
target_link_libraries(simulation gtest)

find_package(Qt5Core    REQUIRED )
find_package(Qt5Widgets REQUIRED )
find_package(Qt5Gui     REQUIRED )

qt5_use_modules(simulation Core Widgets Gui)
qt5_use_modules(simulation_debug Core Widgets Gui)

# Build cg engine
execute_process(COMMAND "make -C ./engine")
