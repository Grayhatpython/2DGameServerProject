#pragma once
#include <future>

//  TEMP
#define UPDATE_THREAD_COUNT 10

class ThreadPool 
{
public:
    ThreadPool();
    ~ThreadPool();

    // job �� �߰��Ѵ�.
    template <typename F, typename... Args>
    std::future<int32> Enqueue(
        F&& f, Args&&... args);

private:

    std::vector<std::thread>            _threads;

    std::queue<std::function<void()>>   _tasks;

    std::condition_variable _cv;
    std::mutex              _lock;

    // ��� ������ ����
    bool _stop;

    // Worker ������
    void WorkerThread();
};


template <typename F, typename... Args>
std::future<int32> ThreadPool::Enqueue(F&& f, Args&&... args)
{
    if (_stop) {
        throw std::runtime_error("ThreadPool ��� ������");
    }

    auto task = std::make_shared<std::packaged_task<int32()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    auto ret = task->get_future();
    {
        std::lock_guard<std::mutex> lock(_lock);
        _tasks.push([task]() { (*task)(); });
    }
    _cv.notify_one();

    return ret;
}