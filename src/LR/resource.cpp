#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>

// struct Resource {
//  private:
//   struct Self {
//     char *p;
//     // FILE *p;
//     Self()
//     {
//       puts("分配资源");
//       p = (char *)malloc(1);
//     }
//     Self(Self const &)  // 拷贝构造函数
//     {
//       puts("复制资源");

//       p = (char *)malloc(1);
//     }
//     // Self(Self &&) = delete;
//     ~Self()
//     {
//       puts("释放资源");
//       free(p);
//     }
//   };
//   std::shared_ptr<Self> self_;
//   Resource(std::shared_ptr<Self> self) { self_ = self; }

//  public:
//   Resource() : self_(std::make_shared<Self>()) {}
//   //   Resource(Resource const &that) { self_ =
//   std::make_shared<self>(*that.self_); } Resource(Resource const &) =
//   default; Resource clone() const { return std::make_shared<Self>(*self_); }
//   void speak() const
//   {
//     printf("Resource::speak() %p\n", self_->p);
//     // self_->p = nullptr;
//   }
//   //   ~Resource() { puts("Resource::~Resource()"); }
// };
// void proc(std::shared_ptr<FILE> p)
// {
//   fgetc(p.get());
//   puts("proc()");
// }
// struct CustomDeleter {
//   void operator()(FILE *p) { fclose(p); }
// };
// void fun(Resource x)
// {
//   x.speak();
//   auto y = x.clone();  // 深拷贝,指向不同对象了，会复制资源
//   //   auto y = x;  // 浅拷贝，不会复制资源
//   y.speak();
// }

// P-IMPL模式，加速编译
struct Resource {
 private:
  FILE *p;
  struct Impl;
  std::unique_ptr<Impl> impl;

 public:
  Resource();
  Resource(Resource const &) = delete;
  ~Resource();
  void speak();
};
struct Resource::Impl {
  FILE *p;
  Impl()
  {
    puts("open file");
    p = fopen("test.txt", "r");
  }

  ~Impl()
  {
    puts("释放资源");
    fclose(p);
  }
  void speak() const { printf("使用文件 %p\n", p); }
};
Resource::Resource() : impl(std::make_unique<Impl>()) {}
Resource::~Resource() = default;
void Resource::speak() { impl->speak(); }
template <class Callback>
struct Finally {
  Callback onExit;
  //   std::function<void()> onExit;
  bool valid;
  Finally() : valid(false) {}
  explicit Finally(Callback onExit) : onExit(onExit), valid(true) {}
  Finally(Finally &&that) noexcept
      : onExit(std::move(that.onExit)), valid(that.valid)
  {
    puts("move");
    that.valid = false;
  }
  // 移动赋值函数：
  Finally &operator=(Finally &&that) noexcept
  {
    if (this != &that) {  // 避免自赋值
      if (valid) {
        onExit();
      }
      onExit = std::move(that.onExit);
      valid = that.valid;
      that.valid = false;
    }
    return *this;
  }
  ~Finally()
  {
    if (valid)  // 判断是否已经被移动
      onExit();
    else {
      puts("避免了一次回调");
    }
  }

  void cancel() { valid = false; }
  void trigger()
  {
    if (valid) onExit();
    valid = false;
  }
};
// auto ffff()
// {
//   auto cb = std::make_shared<Finally>([] { puts("finally"); });
//   return cb;
// }
// Finally ffff2()
// {
//   Finally cb([] { puts("finally2"); });
//   return std::move(cb);
// }

template <class Callback>
Finally(Callback) -> Finally<Callback>;

auto ffff3()
{
  //   auto f = [] { puts("finally3"); };
  // Finally<decltype(f)> cb1(f);
  //   Finally cb(f);
  Finally cb2([] { puts("finally4"); });
  return cb2;
}
int main()
{
  auto r = Resource();

  auto p = std::shared_ptr<FILE>(fopen("test.txt", "r"), fclose);
  //   auto p2 = std::unique_ptr<FILE,
  //   std::default_delete<FILE>>(fopen("test.txt", "r"));
  r.speak();
  //   proc(p);
  //   fun(r);
  auto sp = std::make_shared<int>();
  auto pp = sp.get();
  Finally callback([] { puts("finally"); });
  callback.trigger();
  //   auto cb = ffff();
  //   auto cb2 = ffff3();
  puts("main()");

  // 构造一个空的Finally：
  auto cb3 = ffff3();
  cb3 = decltype(cb3)();
  puts("main 返回");
  if (0) {
    return -1;
  }
  callback.cancel();
  return 0;
}