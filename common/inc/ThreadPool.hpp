#ifndef _THREAD_POOL_HPP_
# define _THREAD_POOL_HPP_

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
private:
    std::vector<std::thread> _threads;
    std::queue<std::function<void(void)> > _tasks;
    std::condition_variable _condition;
    std::mutex _mutex;

    int _stop;

    void ThreadLoop();
public:
    ThreadPool(int nbThread);
    ~ThreadPool();

    template <class R1, typename ... Args>
    void addTask(R1 function, Args&& ... args) {
        addTask(std::bind(std::forward<R1>(function), std::forward<Args>(args) ...));
    }

    template <class R1>
    void addTask(R1 function) {
        _tasks.emplace(function);
        _condition.notify_one();
    }
};

#endif /* _THREAD_POOL_HPP_ */
