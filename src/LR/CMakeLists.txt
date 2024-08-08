cmake_minimum_required(VERSION 3.30.1)
project(MyProject VERSION 1.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
include_directories(${CMAKE_SOURCE_DIR}/../threadpool_1)
# Add the source files
set(SOURCES
Singleton_pattern.cpp
  ${CMAKE_SOURCE_DIR}/../threadpool_1/threadpool.cpp
)
# Create the executable
add_executable(Singleton_pattern ${SOURCES})