# System
cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 14)

# Project
project(NetworkMessengerServer VERSION 1.0)

# Sources
set(source_files src/main.cpp src/Server.cpp)

# Build
add_executable(${CMAKE_PROJECT_NAME} ${source_files})
target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32)

