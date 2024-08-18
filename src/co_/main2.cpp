#include <chrono>
#include <coroutine>
#include <exception>
#include <stdexcept>
#include <variant>

#include "debug.hpp"

struct RepeatAwaiter {
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

struct PreviousAwaiter {
  std::coroutine_handle<> mPrevious;

  bool await_ready() const noexcept { return false; }

  std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
  {
    if (mPrevious)
      return mPrevious;
    else
      return std::noop_coroutine();
  }

  void await_resume() const noexcept {}
};

struct Promise {
  auto initial_suspend() { return std::suspend_always(); }

  auto final_suspend() noexcept { return PreviousAwaiter(mPrevious); }

  void unhandled_exception()
  {
    debug(), "unhandled_exception";
    // mException = std::current_exception();
    mResult.emplace<0>(std::current_exception());
    // throw;//这样就不同写throw了，throw都是在result()中抛出的。
  }

  auto yield_value(int ret)
  {
    // mRetValue = ret;
    mResult.emplace<1>(ret);
    return std::suspend_always();
  }

  void return_value(int ret)
  {
    // mRetValue = ret;
    mResult.emplace<1>(ret);
  }
  int &result()  // 处理协程中的异常
  {
    // 使用optional：
    //  if (mException) {
    //    std::rethrow_exception(mException);
    //  }
    //  return mRetValue.value();
    // 使用variant：
    int *retvalue = std::get_if<1>(&mResult);
    if (!retvalue) [[unlikely]] {
      std::rethrow_exception(std::get<0>(mResult));
    }
    return *retvalue;
  }
  std::coroutine_handle<Promise> get_return_object()
  {
    return std::coroutine_handle<Promise>::from_promise(*this);
  }
  std::exception_ptr mException = nullptr;
  //   std::optional<int> mRetValue{};
  std::variant<std::exception_ptr, int> mResult{};
  //   int mRetValue;
  std::coroutine_handle<> mPrevious = nullptr;
};

struct Task {
  using promise_type = Promise;

  Task(std::coroutine_handle<promise_type> coroutine) : mCoroutine(coroutine) {}

  Task(Task &&) = delete;

  ~Task() { mCoroutine.destroy(); }

  struct Awaiter {
    bool await_ready() const { return false; }

    std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const
    {
      mCoroutine.promise().mPrevious = coroutine;
      return mCoroutine;
    }

    auto await_resume() const
    {
      // return mCoroutine.promise().mRetValue;
      return mCoroutine.promise().result();
    }

    std::coroutine_handle<promise_type> mCoroutine;
  };

  auto operator co_await() const { return Awaiter(mCoroutine); }

  std::coroutine_handle<promise_type> mCoroutine;
};

Task world()
{
  debug(), "world";
  throw std::runtime_error("world出错");
  co_return 41;
}

Task hello()
{
  int i = co_await world();
  debug(), "hello得到world结果为", i;
  co_return i + 1;
}

int main()
{
  debug(), "main即将调用hello";
  Task t = hello();
  debug(), "main调用完了hello";  // 其实只创建了task对象，并没有真正开始执行
  while (!t.mCoroutine.done()) {
    t.mCoroutine.resume();
    debug(), "main得到hello结果为", t.mCoroutine.promise().result();
  }
  return 0;
}