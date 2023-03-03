#ifndef VISTOR_H_HANDLE
#define VISTOR_H_HANDLE
//访问者模式，表示一个作用于某对象结构中的各元素的操作，可用于在不改变
//各元素的前提下定义作用于这些元素的新操作
#include <iostream>

struct ConcreteElement1;
struct ConcreteElement2;

//访问者基类
struct Visitor
{
    virtual ~Visitor(){} //纯虚基类的析构函数必须为虚函数

    virtual void Visit(ConcreteElement1* element) = 0;
    virtual void Visit(ConcreteElement2* element) = 0;
};

//被访问者基类
struct Element
{
    //C++的结构体和类并无太多不同，也有构造函数和成员函数
    virtual ~Element() {}
    virtual void Accept(Visitor& visitor) = 0;
};

//具体的访问者
struct ConcreteVisitor : public Visitor
{
    void Visit(ConcreteElement1* element)
    {
        std::cout << "Visit ConcreteElement1" << std::endl;
    }
    void Visit(ConcreteElement2* element)
    {
        std::cout << "Visit ConcreteElement2" << std::endl;
    }
};

//具体的被访问者
struct ConcreteElement1 : public Element
{
    void Accept(Visitor& visitor)
    {
        visitor.Visit(this);
    }
};
struct ConcreteElement2 : public Element
{
    void Accept(Visitor& visitor)
    {
        visitor.Visit(this);
    }
};








#endif