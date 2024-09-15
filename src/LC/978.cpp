#include <iostream>
#include <vector>

int maxTurbulenceSize(std::vector<int>& arr) {
  int n = arr.size();
  int ans = 1;
  std::vector<std::vector<int>> f(n, std::vector<int>(2, 1));
  for (int i = 1; i < n; i++) {
    f[i][0] = 1;
    f[i][1] = 1;
    if (arr[i] < arr[i - 1]) {
      f[i][0] = f[i - 1][1] + 1;
    } else if (arr[i] > arr[i - 1]) {
      f[i][1] = f[i - 1][0] + 1;
    }
    ans = std::max(ans, std::max(f[i][0], f[i][1]));
  }
  return ans;
}

// 空间压缩，奇偶滚动
int maxTurbulenceSize2(std::vector<int>& arr) {
  int n = arr.size();
  int ans = 1;
  std::vector<std::vector<int>> f(2, std::vector<int>(2, 1));

  for (int i = 1; i < n; i++) {
    f[i % 2][0] = f[i % 2][1] = 1;

    if (arr[i] < arr[i - 1]) {
      f[i % 2][0] = f[(i - 1) % 2][1] + 1;
    } else if (arr[i] > arr[i - 1]) {
      f[i % 2][1] = f[(i - 1) % 2][0] + 1;
    }
    ans = std::max(ans, std::max(f[i % 2][0], f[i % 2][1]));
  }
  return ans;
}

auto main() -> int {
  std::vector<int> arr = {9, 4, 2, 10, 7, 8, 8, 1, 9};
  std::cout << maxTurbulenceSize(arr) << std::endl;
  std::cout << maxTurbulenceSize2(arr) << std::endl;
  return 0;
}