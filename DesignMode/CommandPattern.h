#ifndef COMMANDPATTERN_H_HANDLE
#define COMMANDPATTERN_H_HANDLE
//命令模式，将请求封装为一个对象，将请求的发起者和执行者解耦，支持对请求排队、撤销和重做
#include <functional>
#include <type_traits>

template <typename R = void>
struct CommCommand
{
private:
    std::function<R()> m_f;

public:
    //接收可调用对象的函数包装器
    template<class F,class... Args,class = typename std::enable_if<
            !std::is_member_function_pointer<F>::value>::type>
    void Wrap(F&& f,Args&&... args)
    {
        m_f = [&]{return f(args...);};
    }

    //接收常量成员函数的函数包装器
    template<class R,class C,class... DArgs,class P,class... Args>
    void Wrap(R(C::*f)(DArgs...) const,P&& p, Args &&... args)
    {
        m_f = [&, f]{ return (*p.*f)(args...); };
    }

    //接收非常量成员函数的函数包装器
    template<class R, class C, class... DArgs, class P, class... Args>
    void Wrap(R(C::*f)(DArgs...), P&& p, Args &&... args)
    {
        m_f = [&, f]{ return (*p.*f)(args...); };
    }

    R Excecute()
    {
        return m_f();
    }

};

#endif