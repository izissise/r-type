#ifndef _THREAD_POOL_HPP_
# define _THREAD_POOL_HPP_

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <future>
#include <type_traits>
#include <condition_variable>

class ThreadPool
{

public:
  ThreadPool(size_t nbThread);
  ~ThreadPool();

  template <class FUNC, typename ... Args>
  std::future<typename std::result_of<FUNC(Args...)>::type> addTask(FUNC function, Args&& ... args)
  {
    using returnType = typename std::result_of<FUNC(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<returnType()>>
                (std::bind(std::forward<FUNC>(function), std::forward<Args>(args)...));
    std::future<returnType> futureReturn = task->get_future();

    {
      std::unique_lock<std::mutex> lock(_mutex);
      if (_stop)
        throw std::runtime_error("addTask on stopped ThreadPool.");
      _tasks.emplace([task]()
      {
        (*task)();
      });
    }
    _condition.notify_one();
    return futureReturn;
  };

private:
  void ThreadLoop();

private:
  std::vector<std::thread> _threads;
  std::queue<std::function<void()>> _tasks;
  std::condition_variable _condition;
  std::mutex _mutex;
  bool _stop;
};

#endif /* _THREAD_POOL_HPP_ */
