#include "pch.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool()
    : _stop(false)
{
    _threads.reserve(UPDATE_THREAD_COUNT);
    for (size_t i = 0; i < UPDATE_THREAD_COUNT; ++i)
    {
        _threads.emplace_back([this]() { this->WorkerThread(); });
    }
}

ThreadPool::~ThreadPool()
{
    _stop = true;
    _cv.notify_all();

    for (auto& t : _threads)
    {
        t.join();
    }
}

void ThreadPool::WorkerThread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(_lock);
        _cv.wait(lock, [this]() { return !this->_tasks.empty() || _stop; });
        if (_stop && this->_tasks.empty()) {
            return;
        }

        std::function<void()> job = std::move(_tasks.front());
        _tasks.pop();
        lock.unlock();

        job();
    }
}

