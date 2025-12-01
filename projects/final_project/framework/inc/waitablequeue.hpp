/*****************************************************************************
 * Exercise:    WaitableQueue
 * Date:        01/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Dana Chesner
 * Status:      
 *****************************************************************************/

#ifndef __ILRD_WAITABLE_QUEUE__
#define __ILRD_WAITABLE_QUEUE__

#include <mutex>                // std::timed_mutex
#include <queue>                // std::queue
#include <condition_variable>   // std::condition_variable_any
#include <chrono>               // std::chrono::milliseconds

namespace ilrd
{

template<typename T, typename Container = std::queue<T> >
class WaitableQueue
{
public:
    WaitableQueue() = default;
    ~WaitableQueue() = default;

    void push(const T& value);
    void pop(T* value);
    bool pop(T* value, std::chrono::milliseconds timeout);
    bool empty() const;

    WaitableQueue& operator=(const WaitableQueue& other) = delete;
    WaitableQueue& operator=(WaitableQueue&& other) = delete;
    WaitableQueue(const WaitableQueue& other) = delete;
    WaitableQueue(WaitableQueue&& other) = delete;

private:
    Container m_queue;
    mutable std::timed_mutex m_mutex;
    std::condition_variable_any m_cond_var;
};


template<typename T, typename Container>
void WaitableQueue<T, Container>::push(const T& value)
{
    std::unique_lock<std::timed_mutex> lock(m_mutex);
    m_queue.push(value);
    m_cond_var.notify_one();
}

template<typename T, typename Container>
void WaitableQueue<T, Container>::pop(T* value)
{
    std::unique_lock<std::timed_mutex> lock(m_mutex);
    
    while (m_queue.empty())
    {
        m_cond_var.wait(lock);
    }
    
    *value = m_queue.front();
    m_queue.pop();
}

template<typename T, typename Container>
bool WaitableQueue<T, Container>::pop(T* value, std::chrono::milliseconds timeout)
{
    std::unique_lock<std::timed_mutex> lock(m_mutex);
    
    if (!m_cond_var.wait_for(lock, timeout, [this]() { return !m_queue.empty(); }))
    {
        return false;
    }
    
    *value = m_queue.front();
    m_queue.pop();
    
    return true;
}

template<typename T, typename Container>
bool WaitableQueue<T, Container>::empty() const
{
    std::unique_lock<std::timed_mutex> lock(m_mutex);
    return m_queue.empty();
}

} // namespace ilrd

#endif //__ILRD_WAITABLE_QUEUE__