cmake_minimum_required(VERSION 3.29)

# 项目名称和版本
project(MyCoroutineProject VERSION 1.0 LANGUAGES CXX)

# 设置 C++ 标准为 C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# 设置编译器为 Clang 18
# set(CMAKE_CXX_COMPILER "clang++")


# 添加源文件
add_executable(MyCoroutineProject main.cpp)

# 添加调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 如果需要，可以添加其他编译选项
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")