cmake_minimum_required(VERSION 3.29)
project(MyProject VERSION 1.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 设置编译器
# set(CMAKE_CXX_COMPILER "C:\\msys64\\mingw64\\bin\\clang++.exe")
# 设置编译选项，确保不包含 -fmodules-ts
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# 查找 clang-tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)


# find_package(LLVM REQUIRED CONFIG)
# 设置 clang-tidy 命令，过滤掉不支持的选项
# set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*")

# # 添加一个自定义目标来运行 clang-tidy
# add_custom_target(
#   run_clang_tidy
#   COMMAND ${CLANG_TIDY_COMMAND} -header-filter=.* -system-headers --extra-arg-before=--driver-mode=clang++
#   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
# )

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
# 添加可执行文件
add_executable(MyExecutable ${SRC_FILES})
# 设置目标属性
set_target_properties(
  MyExecutable
  PROPERTIES
    OUTPUT_NAME test
    CXX_STANDARD 23
    CXX_EXTENSIONS OFF
    # CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
    # POSITION_INDEPENDENT_CODE True
)
# target_include_directories(MyExecutable SYSTEM PRIVATE ${CMAKE_CURRENT_BINARY_DIR})