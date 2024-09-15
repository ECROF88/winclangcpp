#include <cstdint>
#include <format>
#include <iostream>
#include <map>
#include <string>
#include <vector>
bool equal(const std::map<std::string, int>& map, std::map<std::string, int>& cur) {
  for (const auto& [key, value] : map) {
    if (cur[key] != value) {
      return false;
    }
  }
  return true;
}
std::vector<int> findSubstring(std::string& s, std::vector<std::string> words) {
  size_t n = s.size();
  size_t m = words.size();
  size_t w = words[0].size();
  std::map<std::string, int> map;
  for (const auto& word : words) {
    map[word]++;
  }
  std::vector<int> res;

  for (int i = 0; i + m * w <= n; i++) {
    std::map<std::string, int> cur;
    std::string sub = s.substr(i, m * w);
    for (size_t j = 0; j < sub.size(); j += w) {
      std::string item = sub.substr(j, w);
      if (map.find(item) == map.end()) {
        break;
      }
      cur[item]++;
    }
    if (cur == map) {
      printf_s("i = %d\n", i);
      res.push_back(i);
    }
  }
  return res;
}

// 滑动窗口+哈希表。优化枚举起点
// 还没看懂？？？？？
std::vector<int> findSubstring2(std::string& s, std::vector<std::string> words) {
  size_t n = s.size();
  size_t m = words.size();
  size_t w = words[0].size();
  std::map<std::string, int> map;
  for (const auto& word : words) {
    map[word]++;
  }
  std::vector<int> res;

  for (int i = 0; i < w; i++) {
    std::map<std::string, int> cur;
    for (int j = i; j + w <= n; j += w) {
      std::string item = s.substr(j, w);
      cur[item]++;

      if (j >= i + m * w) {
        int idx = j - m * w;
        std::string prev = s.substr(idx, w);
        if (cur[prev] == 1) {
          cur.erase(prev);
        } else {
          cur[prev]--;
        }
      }
      if (cur == map) res.push_back(j - (m - 1) * w);
    }
    return res;
  }
}

auto main() -> int {
  std::string s = "barfoothefoobarman";
  std::vector<std::string> words = {"foo", "bar"};
  auto res = findSubstring2(s, words);
  printf("res.size() = %zu\n", res.size());
  for (auto r : res) std::cout << std::format("{} ", r);
  return 0;
}