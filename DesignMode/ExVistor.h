#ifndef EXVISTOR_H_HANDLE
#define EXVISTOR_H_HANDLE
//优化访问者模式，传统访问者模式的接口是不稳定的
//使用可变参函数模板可以使函数接口支持多个参数，使接口稳定化

//利用可变参模板实现一个稳定的接口层
template <typename... Types>
struct ExVisitor;

template <typename T,typename... Types>
struct ExVisitor<T, Types...> : ExVisitor<Types...>
{
    using ExVisitor<Types...>::Visitor;
    virtual void Visitor(const T&) = 0;
};

template <typename T>
struct ExVisitor<T>
{
    virtual void Visitor(const T&) = 0;
};

#endif