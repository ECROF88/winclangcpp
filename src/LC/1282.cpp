#include <iostream>
#include <unordered_map>
#include <vector>

std::vector<std::vector<int>> groupThePeople(std::vector<int>& groupsizes)
{
  std::unordered_map<int, std::vector<int>> map;
  std::vector<std::vector<int>> ans;
  for (int i = 0; i < groupsizes.size(); i++) {
    int x = groupsizes[i];
    map[x].push_back(i);
    if (map[x].size() == x)  // full
    {
      ans.push_back(map[x]);
      map[x].clear();
    }
  }
  return ans;
}

auto main() -> int
{
  std::vector<int> groupsizes = {3, 3, 3, 3, 3, 1, 3};
  std::vector<std::vector<int>> ans = groupThePeople(groupsizes);
  for (auto& group : ans) {
    for (auto& x : group) {
      std::cout << x << " ";
    }
    std::cout << std::endl;
  }
  return 0;
}