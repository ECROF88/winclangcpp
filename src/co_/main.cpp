#include <chrono>
#include <coroutine>
#include <iostream>

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
struct PreviousAwaiter {
  std::coroutine_handle<> mPrevious;
  bool await_ready() const noexcept { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
  {
    if (mPrevious)
      return mPrevious;
    else
      return std::noop_coroutine();  // 如果没有上一个协程，那么直接返回noop_coroutine
  }
  void await_resume() const noexcept {}
};

struct Promise {
  auto initial_suspend() { return std::suspend_always(); }

  auto final_suspend() noexcept  // 这个的作用 在协程结束时调用
  {
    // return std::suspend_always();
    return PreviousAwaiter(mPrevious);  // 如果没有上一个协程，那么直接返回noop_coroutine
  }

  void unhandled_exception() { throw; }

  auto yield_value(int ret)
  {
    mRetValue = ret;
    // return RepeatAwaiter();
    // return SuspendUntilDone();  // 直到done才挂起
    return std::suspend_always();
  }

  void return_value(int ret) { mRetValue = ret; }
  //   void return_void() { mRetValue = 0; }

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
  std::coroutine_handle<> mPrevious = nullptr;
};

struct Task {
  Task(Task&&) = delete;
  ~Task()
  {
    debug(), "Task析构";
    mCoroutine.destroy();
  }
  using promise_type = Promise;

  Task(std::coroutine_handle<promise_type> coroutine) : mCoroutine(coroutine) {}

  std::coroutine_handle<promise_type> mCoroutine;
};
struct WorldTask {
  using promise_type = Promise;
  WorldTask(std::coroutine_handle<promise_type> coroutine) : mCoroutine(coroutine) {}
  WorldTask(WorldTask&&) = delete;
  ~WorldTask()
  {
    debug(), "worldTask析构";
    mCoroutine.destroy();
  }

  struct Awaiter {
    bool await_ready() const noexcept { return false; }
    std::coroutine_handle<> await_suspend(
        std::coroutine_handle<> coroutine) const noexcept
    {
      // 当协程遇到 co_await 表达式时会调用这个方法。
      // 传入的 coroutine 参数是当前正在执行的协程的句柄。
      // if (mCoroutine.promise().mPrevious == nullptr)
      mCoroutine.promise().mPrevious = coroutine;
      // 不加这句话： main得到hello结果为-445748895
      debug(), "world await_suspend";
      return mCoroutine;
    }
    // void await_resume() const noexcept {}
    auto await_resume() const noexcept  // 调用co_await时，会调用await_resume
    {
      return mCoroutine.promise().mRetValue;
      //   return Awaiter{mCoroutine};
    }  // co_await的返回值
    std::coroutine_handle<promise_type> mCoroutine;
  };

  auto operator co_await() { return Awaiter{mCoroutine}; }

  std::coroutine_handle<promise_type> mCoroutine;
};

WorldTask world()
{
  debug(), "world";
  co_yield 99;
  co_return 1234;
}
Task hello()
{
  WorldTask w = world();
  int i = co_await w;
  debug(), "task 获取了world的返回值：", i;
  debug(), "world 返回：", w.mCoroutine.promise().mRetValue;
  i = co_await w;
  debug(), "task 获取了world的返回值：", i;
  //   debug(), "world 返回：",
  //   w.mCoroutine.promise().mRetValue;//把这个加在await_resume里面上面就能用i获取返回值
  debug(), "world finish";
  debug(), "hello 42";
  co_yield 42;
  debug(), "hello 22";
  co_yield 12;
  //   co_await w;
  debug(), "hello 12";
  co_yield 6;
  debug(), "hello 结束";
  i++;
  co_return i;
}

int main()
{
  debug(), "main即将调用hello";
  Task t = hello();
  debug(), "main调用完了hello";
  while (!t.mCoroutine.done()) {
    t.mCoroutine.resume();
    debug(), "main得到hello结果为", t.mCoroutine.promise().mRetValue;
  }
  //   debug(), "main得到hello结果为", t.mCoroutine.promise().mRetValue;
  return 0;
}