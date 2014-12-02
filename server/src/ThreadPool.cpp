#include <iostream>
#include <bits/stl_list.h>
#include "ThreadPool.hpp"

void test()
{
    std::cout << "test" << std::endl;
}

void ThreadPool::ThreadLoop() {
    do {
        std::function<void(void)> task;

        {
            std::unique_lock<std::mutex> lock(_mutex);
            _condition.wait(lock, [this]() { return _stop || _tasks.empty() == false; });
            if (_stop) {
                return;
            }
            task = std::move(_tasks.front());
            _tasks.pop();
        }
        task();
    } while (1);
}

ThreadPool::ThreadPool(int nb_thread) : _minThreadNumber(nb_thread), _stop(false)
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

    std::vector<std::thread>::iterator it = _threads.begin();
    for ( ;it != _threads.end(); it++) {
        it->join();
    }
}

void ThreadPool::addTask(std::function<void(void) > function)
{

}
