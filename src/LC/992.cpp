#include <iostream>
#include <unordered_map>
#include <vector>

#define val const auto
#define var auto
void find(std::vector<int> &nums, std::vector<int> &arr, val k) {
  val n = nums.size();
  std::unordered_map<int, int> cnt;
  var i = 0;
  var j = 0;
  var sum = 0;
  // 滑动窗口，找左边界，i是左边界，j是nums的下标
  for (j = 0; j < n; j++) {
    if (++cnt[nums[j]] == 1) sum++;
    while (sum > k) {
      if (--cnt[nums[i]] == 0) sum--;
      i++;
      std::cout << std::format("i:{} cnt[nums[i]]:{} \n", i, cnt[nums[i]]);
    }
    if (sum == k) arr[j] = i;
  }
  // for (var i = 0; i < n; i++) {
  //   std::cout << std::format("{} ", arr[i]);
  // }
  // std::cout << std::endl;
}
int subarrayWithDistinct(std::vector<int> &nums, val k) {
  val n = nums.size();
  std::vector<int> lower(n, 0), upper(n, 0);
  var ans = 0;
  find(nums, lower, k);
  find(nums, upper, k - 1);

  for (var i = 0; i < n; i++) {
    ans += upper[i] - lower[i];
  }
  return ans;
}
auto main() -> int {
  std::vector<int> nums = {1, 2, 1, 2, 3};
  int k = 2;
  std::cout << subarrayWithDistinct(nums, k) << std::endl;
  return 0;
}