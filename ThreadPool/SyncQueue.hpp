#ifndef SYNCQUEUE_H_HANDLE
#define SYNCQUEUE_H_HANDLE
//同步队列，用于线程池共享数据安全
#include <list>
#include <mutex> //互斥锁
#include <thread> //线程
#include <condition_variable>//条件变量
#include <iostream>

using namespace std;

template <typename T>
class SyncQueue
{
public:
    SyncQueue(int maxSize) : m_maxSize(maxSize), m_needStop(false) { }
    void Put(const T& x)
    {
        Add(x);
    }

    void Put(const T&& x)
    {
        Add(std::forward<T>(x));
    }
    void Take(std::list<T>& list)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        //如果缓冲队列为空，无法取出list，则会堵塞在此，直到其它线程往队列添加成员为止
        m_notEmpty.wait(locker, [this]{return m_needStop || NotEmpty(); });

        if(m_needStop)
            return;
        list = std::move(m_queue);
        //取出之后，则发送队列不为空的消息（条件变量其实就是一种线程通信），如果有其它线程因为队列满了而阻塞，
        //收到不为空的消息后，则会释放阻塞，往队列添加成员
        m_notFull.notify_one();
    }
    void Take(T& t)
    { 
        std::unique_lock<std::mutex> locker;
        m_notEmpty.wait(locker, [this]{return m_needSop || NotEmpty(); });

        if(m_needStop)
            return;
        t = m_queue.front();
        m_queue.pop_front();
        //使用notify_one会唤醒一个被阻塞的线程，我们的队列模型为单生产者单消费者，所以唤醒一个肯定足够
        m_notFull.notify_one();
    }
    void Stop() 
    {
        {
            //lock_guard为局部智能锁，会在出局部声明周期后自动释放，所以放在一个中括号中限定局部
            std::lock_guard<std::mutex> locker(m_mutex);
            m_needStop = true;
        }
    }
    bool Empty()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    }
    bool Full()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size() == m_maxSize;
    }
    size_t Size()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size(); //为什么list读操作也要加锁呢，不是线程安全的吗？
    }
    int Count()
    {
        return m_queue.size();
    }

private:
    bool NotFull() const //常成员函数，函数边辉修改成员变量的值
    {
        bool full= m_queue.size() >= m_maxSize;
        if(full)
            cout << "queue is Full,please wait a minute!" << endl;
        return !full;
    }
    bool NotEmpty() const 
    {
        bool empty = m_queue.empty();
        if(empty)
            cout << "queue is empty,please wait a minute!" << endl;
        return !empty;
    }

    template <typename T>
    void Add(F&& x)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notFull.wait(locker, [this]{ return m_needSop || NotFull(); });
        if(m_needStop)
            return;
        m_queue.push_back(std::forward<F>(x));
        m_notEmpty.notify_one();
    }

private:
    std::list<T> m_queue; //缓冲区
    std::mutex m_mutex;   //互斥量和条件变量结合，为生产者消费者模型
    std::condition_variable m_notEmpty; //不为空的条件变量
    std::condition_variable m_notFull;  //没有满的条件变量
    int m_maxSize;
    bool m_needStop;
};

#endif