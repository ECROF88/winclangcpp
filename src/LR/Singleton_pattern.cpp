#include <atomic>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <print>

#include "../threadpool_1/threadpool.h"

// 单例
struct S;
struct S {
private:
  static std::atomic<S*> instance;
  //   S* instance = nullptr;
  S() = default;
  static std::mutex isInit;
  static int value;

public:
  S(const S&) = delete;
  S& operator=(const S&) = delete;

  static S* getInstance()
  {
    S* temp = instance.load(std::memory_order_relaxed);
    // std::atomic_thread_fence(std::memory_order_acquire);
    if (temp == nullptr) {
      std::lock_guard<std::mutex> locker(isInit);
      temp = instance.load(std::memory_order_relaxed);
      if (temp == nullptr) {
        temp = new S();
        value++;
        // std::atomic_thread_fence(std::memory_order_release);
        instance.store(temp, std::memory_order_release);
      }
    }
    return temp;
  };

  int return_value() { return value; }
};
std::atomic<S*> S::instance{nullptr};
std::mutex S::isInit;
int S::value = 0;

int main()
{
  ThreadPool pool(4, 8);

  for (int i = 0; i < 100; i++) {
    pool.addTask([]() {
      S* s = S::getInstance();
      printf_s("value: %d \n", s->return_value());
    });
  }
  std::cout << __cplusplus;  // 202400
                             //   std::println("Hello, World!");
  getchar();
  return 0;
}