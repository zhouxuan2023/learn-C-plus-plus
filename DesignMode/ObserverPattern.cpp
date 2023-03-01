#include "ObserverPattern.h"

// 附加观察者
void Subject::Attach(Observer *o)
{
    _observers->push_back(o);
}

// 移除观察者
void Subject::Detach(Observer *o)
{
    _observers->remove(o);
}

// 通知所有的观察者
void Subject::Notify()
{
    for (auto it = _observers->begin(); it != _observers->end(); ++it)
    {
        //(*it)->Updata(this);
        Observer* observer = const_cast<Observer*>(*it);
        observer->Updata(this);
    }
}
