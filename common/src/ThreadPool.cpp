#include <iostream>
#include <algorithm>
#include "ThreadPool.hpp"

void ThreadPool::ThreadLoop() {
    do {
        std::function<void(void)> task;

        {
            std::unique_lock<std::mutex> lock(_mutex);
            _condition.wait(lock, [this]() { return _stop || !_tasks.empty(); });
            if (_stop && _tasks.empty()) {
                return;
            }
            task = std::move(_tasks.front());
            _tasks.pop();
        }
        task();
    } while (1);
}

ThreadPool::ThreadPool(int nb_thread) : _stop(false)
{
    for (int i = 0; i < nb_thread; i++)
    {
        _threads.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
    }
}

ThreadPool::~ThreadPool()
{
    _stop = true;
    _condition.notify_all();
    std::for_each(_threads.begin(), _threads.end(), [](std::thread &t) { t.join(); });
}
