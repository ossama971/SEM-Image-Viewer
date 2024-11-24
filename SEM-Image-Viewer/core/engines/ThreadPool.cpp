#include "ThreadPool.h"

std::size_t
    ThreadPool::configured_threads = std::thread::hardware_concurrency() > 0
                                         ? std::thread::hardware_concurrency()
                                         : 1;
std::unique_ptr<ThreadPool> ThreadPool::m_instance = nullptr;
std::once_flag ThreadPool::init_instance_flag;

ThreadPool::ThreadPool() {
  for (int i = 0; i < ThreadPool::configured_threads; i++) {
    pool.emplace_back(&ThreadPool::run, this);
  }
}

void ThreadPool::join() {
  is_active = false;
  cv.notify_all();
  for (auto &th : pool) {
    if (th.joinable())
      th.join();
  }
}

ThreadPool::~ThreadPool() {
  join();
}

void ThreadPool::post(std::packaged_task<void()> job) {
  std::unique_lock lock(guard);
  pending_jobs.emplace_back(std::move(job));
  cv.notify_one();
}

void ThreadPool::run() noexcept {
  while (is_active) {
    thread_local std::packaged_task<void()> job;
    {
      std::unique_lock lock(guard);
      cv.wait(lock, [&] { return !pending_jobs.empty() || !is_active; });
      if (!is_active)
        break;
      job.swap(pending_jobs.front());
      pending_jobs.pop_front();
    }
    job();
  }
}

ThreadPool &ThreadPool::instance() {
  std::call_once(init_instance_flag,
                 [&]() { m_instance.reset(new ThreadPool()); });
  return *m_instance;
}
