#include <iostream>
#include "SingletonPattern.h" //单例模式
#include "Events.h"           //事件，观察者模式
#include "Vistor.h"           //访问者模式
#include "ExVistor.h"         //改进访问者模式

using namespace std;

int main()
{

    return 0;
}

namespace np_ExVisitor{
struct stA;
struct stB;

struct Base
{
    // 定义通用的访问者类型，它可以访问sta和stB
    typedef ExVisitor<stA, stB> myExVisitor;
    virtual void Accept(myExVisitor &) = 0;
};

struct stA : Base
{
    double val;
    void Accept(Base::myExVisitor &v)
    {
        v.Visitor(*this);
    }
};

struct stB : Base
{
    int val;
    void Accept(Base::myExVisitor &v)
    {
        v.Visitor(*this);
    }
};

//通过接口我们就可以实现访问者类PrintVisitor，它可以访问stA、stB的元素
struct PrintVisitor : Base::myExVisitor
{
    void Visitor(const stA &a)
    {
        cout << "from stA:" << a.val << endl;
    }
    void Visitor(const stB &b)
    {
        cout << "form stB:" << b.val << endl;
    }
};

int main()
{
    PrintVisitor vis;
    stA a;
    a.val = 8.97;
    stB b;
    b.val = 8;
    Base* base = &a;//以子类初始化父类指针
    base->Accept(vis);
    base = &b;
    base->Accept(vis);

    return 0;
}
}

namespace np_Vistor
{
    int main()
    {
        ConcreteVisitor v;
        std::unique_ptr<Element> emt1(new ConcreteElement1()); // 使用智能指针做内存管理
        std::unique_ptr<Element> emt2(new ConcreteElement2());

        emt1->Accept(v);
        emt2->Accept(v);

        return 0;
    }
}

namespace np_Events
{
    struct stA
    {
        int a, b;
        void print(int a, int b)
        {
            cout << "stA::print accept the event:";
            cout << a << "," << b << endl;
        }
    };
    void print(int a, int b)
    {
        cout << "print accept the event:";
        cout << a << "," << b << endl;
    }

    int main()
    {
        Events<std::function<void(int, int)>> myevent;

        auto key = myevent.Connect(print); // 以函数的方式注册观察者
        stA t;
        auto lamdakey = myevent.Connect([&t](int a, int b)
                                        {
                                            t.a = a;
                                            t.b = b; // 该lamda表达式为观察者，当有事件发生时，会调用该表达式，表达式会将传递进来的ab赋值给捕获的输入参数t
                                        });          // lamda注册
        // std::function注册，bind可以打包，placeholders::_1为占位符
        std::function<void(int, int)> f = std::bind(&stA::print, &t, std::placeholders::_1, std::placeholders::_2);
        myevent.Connect(f);
        int a = 1, b = 2;
        myevent.Notify(a, b); // 广播事件

        myevent.Disconnect(key);
        myevent.Disconnect(lamdakey);

        return 0;
    }

}

namespace np_SingletonPattern
{
    struct A
    {
        A(const string &) { cout << "lvaue" << endl; }
        A(string &&x) { cout << "rvaue" << endl; }
    };

    struct B
    {
        B(const string &) { cout << "lvaue" << endl; }
        B(string &&x) { cout << "rvaue" << endl; }
    };

    struct C
    {
        C(int x, double y) {}
        void Fun() { cout << "test" << endl; }
    };

    int main()
    {
        cout << "hello Design Mode!" << endl;

        string str = "bb";
        // 创建A类型的单例
        Singleton<A>::Instance(str);
        // 创建B类型的单例,临时变量str被move之后变成右值，然后可以根据移动语义来避免复制
        Singleton<B>::Instance(std::move(str)); //??为什么重载不明确？？
        // 创建C的单例,C类型构造函数包含两个参数
        Singleton<C>::Instance(1, 3.14);
        Singleton<C>::GetInstance()->Fun();

        // 释放单例
        Singleton<A>::DestroyInstance();
        Singleton<B>::DestroyInstance();
        Singleton<C>::DestroyInstance();

        return 0;
    }

} // SingletonPattern
