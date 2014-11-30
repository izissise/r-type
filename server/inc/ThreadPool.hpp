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
    int _minThreadNumber;
public:
    ThreadPool(int nbThread);
    ~ThreadPool();
    void addTask(std::function<void(void) >);
};

#endif /* _THREAD_POOL_HPP_ */