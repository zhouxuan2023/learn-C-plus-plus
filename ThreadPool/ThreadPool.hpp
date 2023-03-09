#ifndef THREADPOOL_H_HANDLE
#define THREADPOOL_H_HANDLE

#include <list>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include "SyncQueue.hpp"

const int MaxTaskCount = 100;

class ThreadPool
{
public:
    using Task = std::function<void()>;

    ThreadPool(int numThreads = std::thread::hardware_concurrency()) : m_queue(MaxTaskCount)
    {
        Start(numThreads);
    }
    ~ThreadPool(void) 
    {
        Stop();
    }
    void Stop() 
    { 
        //保证多线程情况下只调用一次StopThreadGroup函数
        std::call_once(m_flag, [this]{ StopThreadGroup(); });
    }
    void AddTask(Task&& task) { }
    void AddTask(const Task& task) { }

private:
    void Start(int numThreads)
    {
        
    }

    void RunInThread()
    {
        while(m_running)
        {

        }
    }

    void StopThreadGroup()
    {

    }


    std::list<std::shared_ptr<std::thread>> m_threadgroup; //处理任务的线程组
    SyncQueue<Task> m_queue; //同步队列
    atomic_bool m_running;
    std::once_flag m_flag;

};

#endif