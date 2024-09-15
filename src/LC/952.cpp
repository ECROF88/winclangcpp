#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

static int N = 20010;
// std::vector<int> nums(N);
int ans = 1;
static int p[20010];  // root
// static int sz[20010];  //
static std::vector<int> sz(N, 1);
// static std::vector<int> p;
int find(int x)
{
  if (p[x] != x) p[x] = find(p[x]);
  return p[x];
}
void merge(int a, int b)
{
  if (find(a) == find(b)) return;
  sz[find(a)] += sz[find(b)];
  p[find(b)] = p[find(a)];
  std::cout << "merge: " << a << " " << b << " " << sz[find(a)] << std::endl;
  ans = std::max(ans, sz[find(a)]);
}
void add(std::unordered_map<int, std::vector<int>>& map, int key, int val)
{
  map[key].push_back(val);  // key 是因子，val 是索引
}
int largestComponentSize(std::vector<int>& nums)
{
  size_t n = nums.size();
  std::unordered_map<int, std::vector<int>> mp;
  for (int i = 0; i < n; i++) {
    int cur = nums[i];
    for (int j = 2; j * j <= cur; j++) {
      if (cur % j == 0) {
        add(mp, j, i);
      }
      while (cur % j == 0) cur /= j;
    }
    if (cur > 1) add(mp, cur, i);
  }
  for (int i = 0; i <= n; i++) {
    p[i] = i;
  }
  for (auto& pair : mp) {
    for (int i = 1; i < pair.second.size(); i++) {
      merge(pair.second[0], pair.second[i]);
    }
  }
  return ans;
}

auto main() -> int
{
  std::vector<int> nums = {4, 6, 15, 35};
  std::cout << largestComponentSize(nums) << std::endl;
  return 0;
}
