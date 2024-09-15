#include <format>
#include <iostream>
#include <ranges>
#include <vector>

constexpr std::vector<int> fun() {
  return std::vector<int>{1, 2, 3, 4, 5, 6};
}
int main() {
  auto v = fun();

  auto res = v | std::ranges::views::filter([](int &x) { return x % 2 == 0; });
  for (auto x : res) {
    std::cout << std::format("{}", x);
  }
}