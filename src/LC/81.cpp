#include <cstddef>
#include <cstdio>
#include <vector>

#define val const auto
#define var auto
int find(std::vector<int>& nums, auto l, auto r, int target) {
  while (l < r) {
    var mid = (l + r) >> 1;
    if (nums[mid] >= target) {
      r = mid;
    } else {
      l = mid + 1;
    }
  }
  return nums[r] == target ? r : -1;
}
bool search(std::vector<int>& nums, int target) {
  val n = nums.size();
  size_t l = 0, r = n - 1;
  while ((l < r && nums[0] == nums[r])) r--;

  while (l < r) {
    size_t mid = (l + r + 1) >> 1;
    if (nums[mid] >= nums[0]) {
      l = mid;
    } else {
      r = mid - 1;
    }
  }
  size_t idx = n;
  if (nums[r] >= nums[0] && r + 1 < n) idx = r + 1;

  int ans = find(nums, 0, idx - 1, target);
  if (ans != -1) return true;
  ans = find(nums, idx, n - 1, target);
  return ans != -1;
}

auto main() -> int {
  std::vector<int> nums = {2, 5, 6, 0, 0, 1, 2};
  int target = 3;
  if (search(nums, target)) {
    printf("true\n");
  } else {
    printf("false\n");
  }
  return 0;
}