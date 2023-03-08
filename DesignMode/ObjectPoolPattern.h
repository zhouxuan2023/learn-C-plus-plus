#ifndef OBJECTPOOLPATTERN_H_HANDLE
#define OBJECTPOOLPATTERN_H_HANDLE
//设计模式：对象池模式
//对象池对于创建开销比较大的对象来说很有意义，为了避免重复创建开销比较大的对象，
//可以通过对象池来优化
#include <string>
#include <functional>
#include <memory>
#include <map>

using namespace std;

//因此现定义一个不可复制的基类方便继承
class NonCopyable
{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete; // 使用delete关键字可以禁用复制构造
    //禁用赋值构造函数
    NonCopyable& operator = (const NonCopyable&) = delete;
};

template <typename T>
class ObjectPool : NonCopyable //继承基类禁用复制构造和赋值构造
{
    template <typename... Args>
    using Constructor = std::function<std::shared_ptr<T>(Args...)>; //using为typedef的另一种替代用法

public:
    //默认创建多少个对象
    template <typename... Args>
    void Init(size_t num,Args&&... args)
    {
        if(num <= 0 || num > MaxObjectNum)
            throw std::logic_error("object num out of range.");
        //typeid用于返回指针或引用所指对象的实际类型
        auto constructName = typeid(Constructor<Args...>).name();
        for(size_t i = 0; i < num; i++)
        {
            m_object_map.emplace(constructName,shared_ptr<T>(
                //基于forward的完美转发特性，使用传入的可变长度参数包，可以构建出对象实际类型，并使用shared_ptr来做管理
                new T(std::forward<Args>(args)...), [this, constructName](T* p)
                {
                    return createPtr<T>(string(constructName),args...);
                }
            ));
        }
    }

    //实现内部接口
    std::shared_ptr<T> creatPtr(std::string& constructName, Args... args)
    {
        return std::shared_ptr<T>(new T(args...), [constructName, this](T* t)
        {
            if(needClear)
                delete[] t;
            else
                m_object_map.emplace(constructName, std::shared_ptr<T>(t));
        }
        );
    }

    //从对象池中获取一个对象
    template <typename... Args>
    std::shared_ptr<T> Get()
    {
        string constructName = typeid(Constructor<Args...>).name();

        auto range = m_object_map.equal_range(constructName);
        for(auto it = range.frist; it != range.second; ++it)
        {
            auto ptr = it->second;
            m_object_map.erase(it);
            return ptr;
        }

        return mullptr;
    }


private:
    std::multimap<std::string, std::shared_ptr<T>> m_object_map;//需要使用一个map成员函数来做对象池所需的映射
    bool needClear;
}





#endif