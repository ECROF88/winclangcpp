// npoTuymep

#include <iostream>

int main()
{
  const int len1 = 5;
  constexpr int len2 = 10;
  int arr1[len1] = {1, 2, 3, 4, 5};
  int arr2[len2] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  std::cout << "arr1: index 1 = " << arr1[1] << std::endl;
  std::cout << "arr2: index 1 = " << arr2[1] << std::endl;
}
