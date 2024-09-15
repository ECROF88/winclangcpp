#include "threadpool.h"

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <utility>
template <class Self, class Fn>
auto mybind(Self self, Fn&& fn, auto... args) {
  return [self = std::move(self), fn, args...](auto... params) { fn(args...); };
}

ThreadPool::ThreadPool(int min, int max)
    : max_Thread(max), min_Thread(min), current_Thread(min), idle_Thread(min), stop(false) {
  // 初始化线程池
  idle_Thread = current_Thread = min;
  manager_thread = new thread(&ThreadPool::manager, this);
  std::cout << "number of current threads: " << current_Thread << std::endl;
  // 初始化工作线程
  for (int i = 0; i < min; i++) {
    thread t(&ThreadPool::worker, this);
    worker_map.insert(std::make_pair(t.get_id(), std::move(t)));
  }
}
ThreadPool::~ThreadPool() {
  stop = true;
  condition.notify_all();
  for (auto& it : worker_map) {
    thread& t = it.second;
    if (t.joinable()) {
      cout << "(析构) 线程 " << t.get_id() << " 将要退出了..." << endl;
      t.join();
    } else {
      cout << "线程不可join" << endl;
    }
  }
  if (manager_thread->joinable()) {
    manager_thread->join();
  }
  std::cout << "manager died" << std::endl;
  delete manager_thread;
}

void ThreadPool::manager() {
  while (!stop.load()) {
    // 管理者线程
    // 调节线程数量
    // 1. 当任务队列中任务数大于空闲线程数时，增加线程
    // 2. 当空闲线程数大于最小线程数时，减少线程
    // 3. 当线程数大于最大线程数时，减少线程
    // 4. 当线程数小于最小线程数时，增加线程
    this_thread::sleep_for(chrono::milliseconds(10));
    // std::cout << "manager_thread: " << std::this_thread::get_id() << std::endl;
    int idle = idle_Thread.load();
    int cur = current_Thread.load();

    if (idle > cur / 2 && cur >= min_Thread) {
      // 减少线程
      exit_thread.store(2);
      condition.notify_all();
      std::lock_guard<std::mutex> locker(exit_thread_mux);
      for (auto id : exit_thread_v) {
        auto it = worker_map.find(id);
        if (it != worker_map.end()) {
          std::cout << "线程被管理销毁 id=" << it->first << std::endl;
          it->second.join();  // 等待线程结束
          worker_map.erase(it);
        }
      }
      exit_thread_v.clear();
    } else if (idle == 0 && cur < max_Thread && !stop.load()) {
      // 增加线程
      thread t(&ThreadPool::worker, this);
      std::cout << "管理增加线程 id=" << t.get_id() << std::endl;
      worker_map.insert(std::make_pair(t.get_id(), std::move(t)));
      current_Thread++;
      idle_Thread++;
    }
  }
}
void ThreadPool::worker() {
  while (!stop.load()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // 工作线程
    // 从任务队列执行任务 若队列为空则阻塞
    // 还要进行线程同步（互斥锁）
    // 1. 从任务队列中取出任务
    // 2. 执行任务
    // 3. 空闲线程数加一
    std::function<void(void)> task = nullptr;
    {  // 大括号作用：加锁范围
      std::unique_lock<std::mutex> locker(queuemux);
      while (tasks.empty() && !stop) {
        condition.wait(locker);  // 加锁，阻塞
        // 空闲线程太多，需要退出线程
        if (exit_thread.load() > 0) {
          std::lock_guard<std::mutex> locker(exit_thread_mux);
          exit_thread_v.push_back(this_thread::get_id());
          std::cout << "-- 线程任务结束, ID=" << this_thread::get_id() << std::endl;
          exit_thread--;
          current_Thread--;
          idle_Thread--;
          // 需要加锁，锁的目的是保证exit_thread_v操作安全
          return;
          // 之后locker就会析构了
        }
      }
      if (!tasks.empty() && !stop) {
        // std::cout << "取出任务.." << std::endl;
        task = std::move(tasks.front());  // 取出任务
        tasks.pop();
      }
    }
    if (task) {
      idle_Thread--;  // 空闲线程数减一
      task();         // 执行任务
      idle_Thread++;  // 空闲线程数加一
    }
  }
}
void ThreadPool::addTask(std::function<void(void)> task) {
  // 添加任务
  // 1. 将任务加入任务队列queuemux
  // 2. 唤醒工作线程
  {
    std::lock_guard<mutex> locker(queuemux);  // 加锁
    tasks.push(task);
  }
  // 此时locker析构，解锁
  //  唤醒工作线程
  condition.notify_one();
}

void clac(int x, int y) {
  int z = x + y;
  // 随机休眠时间
  std::mt19937 mt(std::random_device{}());
  mt.seed();
  std::uniform_int_distribution<int> dist(2, 5);
  int random_num = dist(mt);
  std::this_thread::sleep_for(std::chrono::seconds(random_num));
  std::cout << z << "=" << x << "+" << y << std::endl;
}
void multiple(int x, int y) {
  int z = x * y;
  std::mt19937 mt(std::random_device{}());
  mt.seed();
  std::uniform_int_distribution<int> dist(1, 3);
  int random_num = dist(mt);
  std::this_thread::sleep_for(std::chrono::seconds(random_num));
  std::cout << z << "=" << x << "*" << y << std::endl;
}
struct Task {
  int task_id;
  std::function<void()> task;
  Task(int i) : task_id{i} {
  }
};
// main:
int main() {
#ifdef _LIBCPP_VERSION
  std::cout << "Using libc++ version " << _LIBCPP_VERSION << std::endl;
#elif defined(__GLIBCXX__)
  std::cout << "Using libstdc++" << std::endl;
#else
  std::cout << "Could not determine standard library" << std::endl;
#endif

  ThreadPool pool(4, 8);
  for (int i = 0; i < 10; i++) {
    Task a_task(i);
    if (i % 2 == 0) {
      a_task.task = std::bind(clac, i, i * 2);
    } else {
      a_task.task = std::bind(multiple, i, i * 2);
    }
    // std::function<void()> obj = std::bind(clac, i, i * 2);
    // std::function<void()> obj = mybind(a_task, a_task.task);
    pool.addTask(mybind(a_task, a_task.task));
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  std::this_thread::sleep_for(std::chrono::seconds(4));
  for (int i = 0; i < 10; i++) {
    Task b_task(i);
    if (i % 2 == 0) {
      b_task.task = std::bind(clac, i, i * 2);
    } else {
      b_task.task = std::bind(multiple, i, i * 2);
    }
    pool.addTask(mybind(b_task, b_task.task));
  }
  getchar();  // 等待线程结束
}
// struct S {
//   int value;
//   S(int v) : value(v) {}
//   void print() { std::cout << "value: " << value << std::endl; }
// };

// int main()
// {
//   ThreadPool pool(2, 8);
//   for (int i = 0; i < 100; i++) {
//     this_thread::sleep_for(chrono::milliseconds(100));
//     pool.addTask([]() {
//       S s(1);
//       s.print();
//     });
//   }
// }