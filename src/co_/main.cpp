#include <chrono>
#include <coroutine>

#include "debug.hpp"
struct SuspendUntilDone {
  bool await_ready() const noexcept { return false; }
  //   constexpr void await_suspend(std::coroutine_handle<> conroutine) const noexcept {}
  std::coroutine_handle<> await_suspend(std::coroutine_handle<> conroutine) const noexcept
  {
    // if (conroutine.done())
    return std::noop_coroutine();
    // else
    //   return conroutine;
    // 如果直接就只返回noop_coroutine，那么会导致协程直接结束，不会再执行后续的代码
  }
  void await_resume() const noexcept {}
};

struct SuspendNever {
  constexpr bool await_ready() const noexcept { return true; }
  constexpr void await_suspend(std::coroutine_handle<> conroutine) const noexcept {}
  constexpr void await_resume() const noexcept {}
};

struct RepeatAwaiter  // awaiter(原始指针) / awaitable(operator->)
{
  bool await_ready() const noexcept { return false; }

  std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
  {
    if (coroutine.done())
      return std::noop_coroutine();
    else
      return coroutine;
  }

  void await_resume() const noexcept {}
};

struct RepeatAwaitable  // awaitable(operator->)
{
  RepeatAwaiter operator co_await() { return RepeatAwaiter(); }  // 转换为awaiter
  /*
    调用co_await时，会调用operator co_await()，返回一个awaiter
  */
};

struct Promise {
  auto initial_suspend() { return std::suspend_always(); }

  auto final_suspend() noexcept { return std::suspend_always(); }

  void unhandled_exception() { throw; }

  auto yield_value(int ret)
  {
    mRetValue = ret;
    return RepeatAwaiter();
    // return SuspendUntilDone();
    // return std::suspend_always();
  }

  //   void return_value(int ret) { mRetValue = ret; }
  void return_void() { mRetValue = 0; }

  //   auto yield_value(int ret)
  //   {
  //     mRetValue = ret;
  //     return std::suspend_always();
  //   }
  std::coroutine_handle<Promise> get_return_object()
  {
    return std::coroutine_handle<Promise>::from_promise(*this);
  }

  int mRetValue;
};

struct Task {
  using promise_type = Promise;

  Task(std::coroutine_handle<promise_type> coroutine) : mCoroutine(coroutine) {}

  std::coroutine_handle<promise_type> mCoroutine;
};
struct WorldTask {
  using promise_type = Promise;
  WorldTask(std::coroutine_handle<promise_type> coroutine) : mCoroutine(coroutine) {}
  std::coroutine_handle<promise_type> mCoroutine;
  bool await_ready() const noexcept { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
  {
    // if (coroutine.done())
    //   return std::noop_coroutine();
    // else
    return mCoroutine;
  }
  void await_resume() const noexcept {}
};

WorldTask world()
{
  debug(), "world";

  co_return;
}
Task hello()
{
  debug(), "开始构建worldtask";
  WorldTask worldTask = world();
  debug(), "完成构建worldtask";
  co_await world();
  debug(), "world finish";
  debug(), "hello 42";
  co_yield 42;
  debug(), "hello 22";
  co_yield 12;
  debug(), "hello 6";
  co_yield 6;
  debug(), "hello 结束";
  //   co_return 22;
  //   co_return 44;
  //   co_return 44;
  co_return;
}

int main()
{
  debug(), "main即将调用hello";
  Task t = hello();
  debug(), "main调用完了hello";
  t.mCoroutine.resume();
  while (!t.mCoroutine.done()) {
    t.mCoroutine.resume();
    debug(), "main得到hello结果为", t.mCoroutine.promise().mRetValue;
  }
  //   debug(), "main得到hello结果为", t.mCoroutine.promise().mRetValue;
  return 0;
}