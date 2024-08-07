// main.cpp
#include <iostream>
#include <print>

int main()
{
  std::cout << "C++ Standard: " << __cplusplus << std::endl;
// 判断并输出编译器信息
#ifdef __clang__
  std::cout << "Compiler: Clang" << std::endl;
  std::cout << "Clang Version: " << __clang_major__ << "." << __clang_minor__ << "."
            << __clang_patchlevel__ << std::endl;
#elif defined(__GNUC__)
  std::cout << "Compiler: GCC" << std::endl;
  std::cout << "GCC Version: " << __GNUC__ << "." << __GNUC_MINOR__ << "."
            << __GNUC_PATCHLEVEL__ << std::endl;
#elif defined(_MSC_VER)
  std::cout << "Compiler: MSVC" << std::endl;
  std::cout << "MSVC Version: " << _MSC_VER << std::endl;
#else
  std::cout << "Unknown Compiler" << std::endl;
#endif
  return 0;
}
