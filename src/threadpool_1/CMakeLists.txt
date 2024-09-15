cmake_minimum_required(VERSION 3.29)
project(threadpool)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_LIBRARY libc++)
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++") # 使用 libc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native ")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
# Add the source files
set(SOURCES
  threadpool.cpp
  threadpool.h
)

# Create the executable
add_executable(threadpool ${SOURCES})