#include <cstddef>
#include <cstring>
#include <iostream>
#include <utility>

struct A {
  // 隐式传参：
  void foo() & { std::cout << "foo()&" << std::endl; }
  void foo() && { std::cout << "foo()&&" << std::endl; }
};
struct B {
  B() = default;
  // 显式传参：
  // 常量左值引用:
  void foo(this const B&) { std::cout << "foo(const this B&)" << std::endl; }
  // 右值引用:
  void foo(this B&&) { std::cout << "foo(this B&&)" << std::endl; }
  // 会发生拷贝:
  //   void foo(this B b) { std::cout << "foo(this B b)" << std::endl; }
  // 甚至this后面的可以不是B:传入A时候先shi
  operator A()
  {
    std::puts("convert->A");
    return {};
  }
  void bar(this A a) { std::cout << "foo(this int a)" << std::endl; }
};
struct C {
  // 模板中this自动推导类型
  template <class T>
  void foo(this T&& t)
  {
    std::cout << "自动推导类型" << std::endl;
  }
};
// 指向显式对象成员函数的指针是普通函数指针

auto fff() {}
auto main() -> int
{
  B b;
  b.foo();             // foo(const this B&)
  std::move(b).foo();  // foo(this B&&)
  b.bar();             // 输出：convert->A foo(this int a)
  C c;
  const C& cc = c;
  cc.foo();            // const c&
  c.foo();             // c&
  std::move(c).foo();  // c&&
}