cmake_minimum_required(VERSION 3.13)  # CMake version check
project(cmpe250-assignment3) # Create project
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

# Add cpp and header files of project root directory as source file
# You can append your custom source files to this variable, eg. MyClass.h MyClass.cpp
set(SOURCE_FILES SeatOperations.cpp SeatOperations.h main.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(cmpe250-assignment3 ${SOURCE_FILES})