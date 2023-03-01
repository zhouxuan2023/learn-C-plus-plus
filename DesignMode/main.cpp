#include <iostream>
#include "SingletonPattern.h"

using namespace std;

struct A
{
    A(const string&){ cout << "lvaue" << endl;}
    A(string&& x){ cout << "rvaue" << endl;}
};

struct B
{
    B(const string&){ cout << "lvaue" << endl;}
    B(string&& x){ cout << "rvaue" << endl;}
};

struct C
{
    C(int x, double y){}
    void Fun(){ cout << "test" << endl;}
};

int main()
{
    cout << "hello Design Mode!" << endl;

    string str = "bb";
    //创建A类型的单例
    Singleton<A>::Instance(str);
    //创建B类型的单例,临时变量str被move之后变成右值，然后可以根据移动语义来避免复制
    Singleton<B>::Instance(std::move(str));//??为什么重载不明确？？
    //创建C的单例,C类型构造函数包含两个参数
    Singleton<C>::Instance(1,3.14);
    Singleton<C>::GetInstance()->Fun();

    //释放单例
    Singleton<A>::DestroyInstance();
    Singleton<B>::DestroyInstance();
    Singleton<C>::DestroyInstance();

    return 0;
}
