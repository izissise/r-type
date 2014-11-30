#include "ThreadPool.hpp"

ThreadPool::ThreadPool(int nb_thread)
{
    _minThreadNumber = nb_thread;
    for (int i = 0; i < nb_thread; i++)
    {
        auto func_thread = [this]() {
            do
            {
                std::unique_lock<std::mutex> lock(this->_mutex);
                this->_condition.wait(lock,
                        [this]
                        {
                            return this->_tasks.empty() == false;
                        });
                std::function<void(void)> task = std::move(this->_tasks.front());
                this->_tasks.pop();
                task();
            } while (1);
        };
        _threads.emplace_back(func_thread);
    }
}

ThreadPool::~ThreadPool()
{

}

void ThreadPool::addTask(std::function<void(void) > function)
{

}
