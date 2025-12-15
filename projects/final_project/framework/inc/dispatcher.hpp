/*****************************************************************************
 * Exercise:    Dispatcher
 * Date:        10/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Menny Markovich
 * Status:      Approved
 *****************************************************************************/

#ifndef __ILRD_DISPATCHER__
#define __ILRD_DISPATCHER__

#include <unordered_set>    // std::unordered_set

namespace ilrd
{

template <typename EVENT>
class Dispatcher;

template <typename EVENT>
class BaseCallback
{
public:
    explicit BaseCallback();
    virtual ~BaseCallback();

    void SetDispatcher(Dispatcher<EVENT>* dispatcher);

    virtual void Notify(const EVENT& event) = 0;
    void NotifyDeath();
    virtual void NotifyDeathEx();

    BaseCallback(const BaseCallback&) = delete;
    BaseCallback& operator=(const BaseCallback&) = delete;

private:
    Dispatcher<EVENT>* m_dispatcher;
}; // class BaseCallback


template <typename EVENT>
class Dispatcher
{
public:
    Dispatcher() = default;
    ~Dispatcher();

    void Subscribe(BaseCallback<EVENT>* callback);
    void UnSubscribe(BaseCallback<EVENT>* callback);
    void Broadcast(const EVENT& event);

    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;
    Dispatcher(Dispatcher&&) = delete;
    Dispatcher& operator=(Dispatcher&&) = delete;

private:
    std::unordered_set<BaseCallback<EVENT>*> m_callbacks;
}; // class Dispatcher

template <typename EVENT, typename OBSERVER>
class Callback : public BaseCallback<EVENT>
{
public:
    using NotifyFunc = void (OBSERVER::*)(const EVENT&);
    using NotifyDeathFunc = void (OBSERVER::*)();

    Callback(OBSERVER& observer,
             NotifyFunc notifyFunc,
             NotifyDeathFunc notifyDeathFunc = nullptr);

    virtual ~Callback() = default;

    virtual void Notify(const EVENT& event) override;
    virtual void NotifyDeathEx() override;


private:
    OBSERVER& m_observer;
    NotifyFunc m_notifyFunc;
    NotifyDeathFunc m_notifyDeathFunc;
}; // class Callback

template <typename EVENT>
BaseCallback<EVENT>::BaseCallback()
    : m_dispatcher(nullptr)
{
}

template <typename EVENT>
BaseCallback<EVENT>::~BaseCallback()
{
    if (m_dispatcher != nullptr)
    {
        m_dispatcher->UnSubscribe(this);
    }
}

template <typename EVENT>
void BaseCallback<EVENT>::SetDispatcher(Dispatcher<EVENT>* dispatcher)
{
    m_dispatcher = dispatcher;
}

template <typename EVENT>
void BaseCallback<EVENT>::NotifyDeath()
{
    m_dispatcher = nullptr;
    NotifyDeathEx();
}

template <typename EVENT>
void BaseCallback<EVENT>::NotifyDeathEx()
{

}

template <typename EVENT>
Dispatcher<EVENT>::~Dispatcher()
{
    // Create a copy because NotifyDeath might modify m_callbacks
    std::unordered_set<BaseCallback<EVENT>*> callbacksCopy = m_callbacks;

    for (BaseCallback<EVENT>* callback : callbacksCopy)
    {
        callback->NotifyDeath();
    }

    m_callbacks.clear();
}

template <typename EVENT>
void Dispatcher<EVENT>::Subscribe(BaseCallback<EVENT>* callback)
{
    m_callbacks.insert(callback);
    callback->SetDispatcher(this);
}

template <typename EVENT>
void Dispatcher<EVENT>::UnSubscribe(BaseCallback<EVENT>* callback)
{
    m_callbacks.erase(callback);
    callback->SetDispatcher(nullptr);
}

template <typename EVENT>
void Dispatcher<EVENT>::Broadcast(const EVENT& event)
{
    // Create a copy because Notify might cause subscribe/unsubscribe
    std::unordered_set<BaseCallback<EVENT>*> callbacksCopy = m_callbacks;

    for (BaseCallback<EVENT>* callback : callbacksCopy)
    {
        callback->Notify(event);
    }
}

template <typename EVENT, typename OBSERVER>
Callback<EVENT, OBSERVER>::Callback(OBSERVER& observer,
                                     NotifyFunc notifyFunc,
                                     NotifyDeathFunc notifyDeathFunc)
    : BaseCallback<EVENT>()
    , m_observer(observer)
    , m_notifyFunc(notifyFunc)
    , m_notifyDeathFunc(notifyDeathFunc)
{
}

template <typename EVENT, typename OBSERVER>
void Callback<EVENT, OBSERVER>::Notify(const EVENT& event)
{
    (m_observer.*m_notifyFunc)(event);
}

template <typename EVENT, typename OBSERVER>
void Callback<EVENT, OBSERVER>::NotifyDeathEx()
{
    if (m_notifyDeathFunc != nullptr)
    {
        (m_observer.*m_notifyDeathFunc)();
    }
}

} // namespace ilrd

#endif // __ILRD_DISPATCHER__