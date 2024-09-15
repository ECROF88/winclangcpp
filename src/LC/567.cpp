/*
判断s2是否包含s1的排列
也就是s1的字符组成的排列之一是s2的子串
*/
#include <string>
#include <vector>
bool check(std::vector<int> &cnt, std::vector<int> &cur) {
  for (int i = 0; i < 26; i++) {
    if (cnt[i] != cur[i]) return false;
  }
  return true;
}
bool checkInclusion(std::string &s1, std::string &s2) {
  auto m = s1.size(), n = s2.size();
  if (m > n) return false;
  std::vector<int> cnt(26, 0);

  for (auto c : s1) {
    cnt[c - 'a']++;
  }
  std::vector<int> cur(26, 0);
  for (int i = 0; i < m; i++) {
    cur[s2[i] - 'a']++;
  }
  // 滑动窗口，每次检查长度为s1的长度的窗口内是否包含s1的排列
  if (check(cnt, cur)) return true;
  for (auto i = m; i < n; i++) {
    cur[s2[i] - 'a']++;
    cur[s2[i - m] - 'a']--;
    if (check(cnt, cur)) return true;
  }
  return false;
}

int main() {
  std::string s1 = "ab";
  std::string s2 = "eidbaooo";
  if (checkInclusion(s1, s2)) {
    printf("true\n");
  } else {
    printf("false\n");
  }
  return 0;
}