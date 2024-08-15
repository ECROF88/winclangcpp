#include <chrono>
#include <coroutine>
#include <exception>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <thread>

template <typename T>
class Task
{
 public:
  struct promise_type;
  using TaskHandle = std::coroutine_handle<promise_type>;

 private:
  TaskHandle hd1;

 public:
  explicit Task(auto h) : hd1{h} {}
  ~Task()
  {
    if (hd1) hd1.destroy();
  }

  Task(const Task&) = delete;
  Task& operator=(const Task&) = delete;

  bool resume()
  {
    if (!hd1 || hd1.done()) return false;
    hd1.resume();
    return true;
  }
  struct promise_type {
    T current_value;
    std::suspend_always yield_value(int value)
    {
      current_value = std::move(value);
      return {};
    }
    auto get_return_object() { return Task{TaskHandle::from_promise(*this)}; }
    // auto initial_suspend() { return std::suspend_always{}; }
    std::suspend_never initial_suspend() { return {}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
    // auto final_suspend() noexcept { return std::suspend_always{}; }
    std::suspend_never final_suspend() noexcept { return {}; }
  };
  struct iterator {
    std::coroutine_handle<promise_type> handle;
    bool operator!=(std::default_sentinel_t) const { return !handle.done(); }
    void operator++() { handle.resume(); }
    int operator*() const { return handle.promise().current_value; }  //
  };
  auto begin() -> iterator { return {hd1}; }
  auto end() -> std::default_sentinel_t { return {}; }
};
struct Timer {
  int duration;
  std::chrono::steady_clock::time_point start_time;
  Timer(int seconds) : duration(seconds), start_time(std::chrono::steady_clock::now()) {}
  bool await_ready()
  {
    return std::chrono::steady_clock::now() - start_time >=
           std::chrono::seconds(duration);
  }
  void await_suspend(std::coroutine_handle<> handle)
  {
    std::thread([this, handle]() {
      std::this_thread::sleep_for(std::chrono::seconds(duration));
      handle.resume();
    }).detach();
  }
  void await_resume() {}
};
Task<int> hello(int m)
{
  std::cout << "hello" << std::endl;
  for (int i = 0; i < m; i++) {
    std::cout << i << std::endl;
    co_await std::suspend_always{};
  }
}
Task<int> wait_three()
{
  while (true) {
    std::cout << "wait three seconds\n";
    co_await Timer(3);
  }
}
Task<int> wait_two()
{
  while (true) {
    std::cout << "          wait one seconds\n";
    co_await Timer(1);
  }
}
Task<int> fib()
{
  int a = 1;
  int b = 1;
  co_yield a;
  co_yield b;
  while (true) {
    int c = a + b;
    co_yield c;
    a = b;
    b = c;
  }
}
struct frame {
  int a;
  int b;
  int c;
  size_t state;
  bool ready = true;
  std::exception_ptr eptr;
  int resume()
  {
    try {
      switch (state) {
        case 0:
          state = 1;
          goto s0;
        case 1:
          state = 2;
          goto s1;
        case 2:
          goto s2;
      }
    s0:
      a = b = 1;
      return a;
    s1:
      return b;
    s2:
      while (true) {
        c = a + b;
        a = b;
        b = c;
        return c;
      }
    } catch (...) {
      this->eptr = std::current_exception();
      return -1;
    }
  }
};
/*
暂停之后只能return的协程叫做非对称协程
协程之间有明确的父子关系
特征：调度器干预所有的调度
*/
/*
解决方法：转移执行权：switch to
对称式协程，协程之间地位相等
比较灵活，难以debug
*/
/*
无栈协程：用结构体存放协程的状态，不同的协程用不同的结构体
典型：aysnc和await，promise的语法糖。显式暂停，内存紧凑，定制结构体，深递归慢（多次申请内存）。
有栈协程：给个容量很大栈内存，什么都放
没有aysnc和await。隐蔽暂停，浪费内存和栈，深递归较快。
*/
int main()
{
  //   Task co = hello(5);
  //   while (co.resume()) {
  //     std::cout << "hello coroutine suspend\n";
  //   }
  //   using std::ranges::views::drop;
  //   using std::ranges::views::take;
  //   std::cout << "FIB" << std::endl;
  //   // test myfib:
  //   frame f{};
  //   for (int i = 0; i < 10; i++) {
  //     std::cout << f.resume() << std::endl;
  //   }

  // test 3s and 1s
  wait_three();
  wait_two();
  //   Task co1 = wait_three();
  //   Task co2 = wait_two();
  //   while (true) {
  //     co1.resume();
  //     co2.resume();
  //     std::this_thread::sleep_for(std::chrono::seconds(100));
  //   }
  std::this_thread::sleep_for(std::chrono::seconds(100));
  return 0;
}