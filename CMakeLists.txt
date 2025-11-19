cmake_minimum_required(VERSION 3.10)
project(ALPR_OpenCV)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable
add_executable(reader reader.cpp)

# Link OpenCV libraries
target_link_libraries(reader ${OpenCV_LIBS})

# Print OpenCV version for debugging
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")