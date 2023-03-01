#ifndef SINGLETONPATTERN_H_HANDLE
#define SINGLETONPATTERN_H_HANDLE
//设计模式中的单例模式

template <typename T>
class Singleton
{
public:
    template<typename... Args>
    static T* Instance(Args... args)
    {
        if(m_pInstance == nullptr)
        {
            m_pInstance = new T(std::forward<Args>(args)...);//forward完美转发
            //m_pInstance = new T(args...);//不使用完美转发，则均为左值引用
        }
        return m_pInstance;
    }
    //获取单例
    static T* GetInstance()
    {
        if(m_pInstance == nullptr)
            throw std::logic_error("the instance is not init,please initialize the instance frist!");
        return m_pInstance;
    }

    static void DestroyInstance()
    {
        delete m_pInstance;
        m_pInstance == nullptr;
    }
private:
    Singleton(void);//单例模式构造、析构函数必须为私有
    virtual ~Singleton(void);
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton&);

private:
    static T* m_pInstance;//单例对象指针

};

template <class T> T* Singleton<T>::m_pInstance = nullptr;

#endif