// Creator:  0000
#include <iostream>
using namespace std;
auto main() -> int
{
  const int MY_CONSTANT = 111000;
  int anotherVariableName = MY_CONSTANT;
  std::cout << anotherVariableName << std::endl;

  std::cout << __cplusplus;
  if (1 < 2) {
    std::cout << 1;
  }
  return 0;
}
