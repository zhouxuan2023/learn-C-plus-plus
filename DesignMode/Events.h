#ifndef EVENTS_H_HANDLE
#define EVENTS_H_HANDLE
//改进观察者模式，与C#中的事件Events类似
#include <iostream>
#include <string>
#include <map>
#include <functional>

//Events类不可复制，因此现定义一个不可复制的基类方便继承
class NonCopyable
{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete; // 使用delete关键字可以禁用复制构造
    //禁用赋值构造函数
    NonCopyable& operator = (const NonCopyable&) = delete;
};

template <typename Func>
class Events : NonCopyable
{
public: 
    Events(){}//模板类定义与声明均在.h文件
    ~Events(){}
    //注册观察者，支持右值引用
    int Connect(Func&& f)
    {
        return Assgin(f);
    }
    //注册观察者
    int Connect(const Func& f)
    {
        return Assgin(f);
    }
    //移除观察者
    void Disconnect(int key)
    {
        m_connections.erase(key);
    }

    //通知所有观察者,使用可变参模板来将接口参数化，利用forward完美转发保证参数类型不变
    template <typename... Args>
    void Notify(Args&&... args)
    {
        for(auto& it : m_connections)
        {
            it.second(std::forward<Args>(args)...);
        }
    }

private:
    //保存观察者
    template <typename F>
    int Assgin(F&& f)
    {
        int k = m_observerId++;
        m_connections.emplace(k,std::forward<F>(f));//forward完美转发
        return k;
    }

    int m_observerId = 0;//观察者对应的编号
    std::map<int,Func> m_connections;//观察者列表

};

#endif