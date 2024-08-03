cmake_minimum_required(VERSION 3.29)
project(test
VERSION 1.0.0.0 
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(MyExecutable 1.cpp)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set_target_properties(
  MyExecutable
  PROPERTIES
    POSITION_INDEPENDENT_CODE True
    WIN32_EXECUTABLE False
    OUTPUT_NAME ${PROJECT_NAME}
    DEBUG_POSTFIX d
    CXX_STANDARD 23
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
)

target_include_directories(MyExecutable SYSTEM PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB SOURCE_FILES src/*)
target_sources(MyExecutable PRIVATE ${SOURCE_FILES})
