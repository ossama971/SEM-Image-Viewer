#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <deque>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <tuple>
#include <type_traits>
#include <vector>

// Credits: https://nixiz.github.io/yazilim-notlari/2023/10/07/thread_pool-en

class ThreadPool {
public:
  ~ThreadPool();
  void post(std::packaged_task<void()> job);
  static ThreadPool &instance();

  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = delete;
  ThreadPool &operator=(ThreadPool &&) = delete;

  void join();

private:
  explicit ThreadPool();
  void run() noexcept;

private:
  std::atomic_bool is_active{true};
  std::vector<std::thread> pool;
  std::condition_variable cv;
  std::mutex guard;
  std::deque<std::packaged_task<void()>> pending_jobs;

  static std::size_t configured_threads; // TODO: make this number configurable
  static std::unique_ptr<ThreadPool> m_instance;
  static std::once_flag init_instance_flag;
};

struct use_future_tag {};

template <class Fn> constexpr auto use_future(Fn &&func) {
  return std::make_tuple(use_future_tag{}, std::forward<Fn>(func));
}

template <class Executor, class Fn> void post(Executor &exec, Fn &&func) {
  using return_type = decltype(func());
  static_assert(std::is_void_v<return_type>,
                "posting functions with return types must be used with "
                "\"use_future\" tag.");
  std::packaged_task<void()> task(std::forward<Fn>(func));
  exec.post(std::move(task));
}

template <class Executor, class Fn>
[[nodiscard]] decltype(auto) post(Executor &exec,
                                  std::tuple<use_future_tag, Fn> &&tpl) {
  using return_type = std::invoke_result_t<Fn>;
  auto &&[_, func] = tpl;
  if constexpr (std::is_void_v<return_type>) {
    std::packaged_task<void()> tsk(std::move(func));
    auto ret_future = tsk.get_future();
    exec.post(std::move(tsk));
    return ret_future;
  } else {
    struct forwarder_t {
      forwarder_t(Fn &&fn) : tsk(std::forward<Fn>(fn)) {}
      void
      operator()(std::shared_ptr<std::promise<return_type>> promise) noexcept {
        promise->set_value(tsk());
      }

    private:
      std::decay_t<Fn> tsk;
    } forwarder(std::forward<Fn>(func));

    auto promise = std::make_shared<std::promise<return_type>>();
    auto ret_future = promise->get_future();
    std::packaged_task<void()> tsk(
        [promise = std::move(promise),
         forwarder = std::move(forwarder)]() mutable { forwarder(promise); });
    exec.post(std::move(tsk));
    return ret_future;
  }
}

#endif // THREAD_POOL_H
