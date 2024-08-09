#include <cstdio>
#include <iostream>
#include <print>
#include <utility>

// CRTP, Curiously Recurring Template Pattern
struct inc_op {
  auto operator++(this auto&& self) { return self.inc(); }
  auto operator++(this auto&& self, int)
  {
    auto copy = self;
    self.inc();
    return copy;
  }
};
struct A : inc_op {
  int value;
  int value2;
  A(int i, int j) : value(i), value2(j) {}
  auto inc()
  {
    value2 += 1;
    value++;
    return *this;
  }
};
auto main() -> int
{
  A a = {100, 200};
  a++;
  ++std::move(a);
  std::cout << a.value << '\n' << a.value2 << std::endl;
  // 简化递归lamda：以斐波那契数列为例
  auto fib = [](this auto&& self, int n) -> int {
    if (n < 2) return 1;
    return self(n - 1) + self(n - 2);
  };
  std::cout << fib(5) << std::endl;

  // 不使用显式传参的lamda递归
  auto fib2 = [](auto&& _fib, int n) -> int {
    if (n < 2) return 1;
    return _fib(_fib, n - 1) + _fib(_fib, n - 2);
  };
  std::cout << fib2(fib2, 5) << std::endl;
  return 0;
}