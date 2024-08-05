#pragma once
/*
    管理者线程 调节线程数量
    工作线程 从任务队列执行任务 若队列为空则阻塞
            还要进行线程同步（互斥锁）
            最大最小线程数
    任务队列 互斥锁
            条件变量
    线程池开关
*/
#include <atomic>
#include <condition_variable>
#include <functional>
#include <map>
#include <queue>
#include <thread>
#include <vector>

using namespace std;
const int MIN_THREAD = 4;
class ThreadPool
{
private:
  std::thread* manager_thread;  // 管理者线程
  //   std::vector<thread> worker_threads;            // 工作线程
  std::map<std::thread::id, thread> worker_map;  // 工作线程
  std::vector<std::thread::id> exit_thread_v;    // 已经退出的线程id
  std::atomic<int> max_Thread;                   // 最大线程数
  std::atomic<int> min_Thread;                   // 最小线程数
  std::atomic<int> current_Thread;               // 当前线程数
  std::atomic<int> idle_Thread;                  // 空闲线程数
  std::atomic<bool> stop;                        // 线程池开关
  std::mutex queuemux;                           // 互斥锁
  std::atomic<int> exit_thread;                  // 退出线程数
  std::queue<std::function<void(void)>> tasks;   // 任务队列
  std::condition_variable condition;  // 条件变量，用于阻塞和唤醒工作线程
  std::mutex exit_thread_mux;         // 互斥锁
  void manager();                     // 管理者线程的函数
  void worker();                      // 工作线程的函数
public:
  ThreadPool(int min = MIN_THREAD, int max = thread::hardware_concurrency());
  ~ThreadPool();
  void addTask(std::function<void(void)> task);
};
