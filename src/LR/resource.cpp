#include <cstdio>
#include <cstdlib>
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
//   //   Resource(Resource const &that) { self_ = std::make_shared<self>(*that.self_); }
//   Resource(Resource const &) = default;
//   Resource clone() const { return std::make_shared<Self>(*self_); }
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
int main()
{
  auto r = Resource();

  auto p = std::shared_ptr<FILE>(fopen("test.txt", "r"), fclose);
  //   auto p2 = std::unique_ptr<FILE, std::default_delete<FILE>>(fopen("test.txt", "r"));
  r.speak();
  //   proc(p);
  //   fun(r);
  return 0;
}