/*****************************************************************************
 * Exercise:    AsyncInjection
 * Date:        17/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#include "async_injection.hpp"
#include "handleton.hpp" // Handleton
namespace ilrd
{

AsyncInjection::AsyncTask::AsyncTask(AsyncInjection& owner)
    : m_owner(owner)
{
}

void AsyncInjection::AsyncTask::Execute()
{
    bool reschedule = m_owner.m_func();
    
    if (reschedule)
    {
        delete &m_owner;
    }
    else
    {
        m_owner.Schedule();
    }
}

AsyncInjection::AsyncInjection(std::function<bool(void)> func,
                               std::chrono::milliseconds interval)
    : m_func(func)
    , m_interval(interval)
    
{
    m_task = std::make_shared<AsyncTask>(*this);
    Schedule();
}

void AsyncInjection::Schedule()
{
    Handleton<Scheduler>::GetInstance()->Add(m_task, m_interval);
}

} // namespace ilrd