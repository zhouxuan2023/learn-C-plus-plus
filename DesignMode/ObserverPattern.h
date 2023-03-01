#ifndef OBSERVERPATTERN_H_HANDLE
#define OBSERVERPATTERN_H_HANDLE
#include <list>
//这种实现不够通用，只有对特定的观察者才能有效，即必须时Observer抽象类的派生类才行，且该观察者类
//不能带参数

class Subject;

//观察者接口类
class Observer{
public:
    virtual ~Observer();
    virtual void Updata(Subject* theChangedSubject) = 0;//需要被覆写的虚函数接口
};

//主题类
class Subject{
public:
    virtual ~Subject();
    virtual void Attach(Observer*);
    virtual void Detach(Observer*);
    virtual void Notify();
protected:
    Subject();
private:
    std::list<Observer*> *_observers;//观察者列表
};

#endif