#include <boost/asio.hpp>
#include <iostream>
#include <vector>

using namespace std;

int lengthofLIS(std::vector<int> nums)
{
  vector<int> dp(nums.size(), 1);
  int max_len = 0;
  for (int i = 0; i < nums.size(); i++) {
    for (int j = 0; j < i; j++) {
      if (nums[i] > nums[j]) {
        dp[i] = max(dp[i], dp[j] + 1);
      }
      //   if (dp[i] > max_len) max_len = dp[i];
    }
  }
  for (int i = 0; i < nums.size(); i++)
    if (dp[i] > max_len) max_len = dp[i];
  return max_len;
}
// method 2 using binary search
int lengthofLIS_by_binary_search(std::vector<int> nums)
{
  vector<int> dp;
  for (int i = 0; i < nums.size(); i++) {
    auto it =
        lower_bound(dp.begin(), dp.end(), nums[i]);  // 找到第一个大于等于nums[i]的元素
    if (it == dp.end()) {
      dp.push_back(nums[i]);
    } else {
      *it = nums[i];
    }
  }
  return dp.size();
}

auto main() -> int
{
  vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18, 20};
  cout << lengthofLIS(nums) << endl;
  cout << lengthofLIS_by_binary_search(nums) << endl;

  return 0;
}