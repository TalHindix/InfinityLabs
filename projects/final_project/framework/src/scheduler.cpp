/*****************************************************************************
 * Exercise:    Scheduler
 * Date:        16/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Dana Chesner
 * Status:      
 *****************************************************************************/

#include <signal.h>    // sigevent
#include <time.h>      // timer_create
#include <cstring>     // memset
#include <iostream>

#include "scheduler.hpp"

namespace ilrd
{

Scheduler::Scheduler()
    : m_waitable_queue()
    , m_timer()
{
    struct sigevent sev;
    std::memset(&sev, 0, sizeof(sev));
    
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = TimerCallback;
    sev.sigev_value.sival_ptr = this;
    
    timer_create(CLOCK_MONOTONIC, &sev, &m_timer);
}

Scheduler::~Scheduler() noexcept
{
    timer_delete(m_timer);
}

void Scheduler::Add(std::shared_ptr<ISTask> task, const Duration delay)
{
    TimePoint exec_time = std::chrono::steady_clock::now() + delay;
        
    std::cout << "Add: pushing to queue..." << std::endl;
    m_waitable_queue.push({task, exec_time});
    std::cout << "Add: pushed! Setting timer..." << std::endl;

    SetTimer(exec_time);
}

void Scheduler::SetTimer(TimePoint exec_time)
{
    auto now = std::chrono::steady_clock::now();
    auto delay = exec_time - now;
    

    if (delay.count() < 0)
    {
        delay = std::chrono::nanoseconds(1);
    }
    
    auto delay_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(delay);
    
    struct itimerspec its;
    std::memset(&its, 0, sizeof(its));
    
    its.it_value.tv_sec = delay_ns.count() / 1000000000;
    its.it_value.tv_nsec = delay_ns.count() % 1000000000;
    
    timer_settime(m_timer, 0, &its, nullptr);
}

void Scheduler::TimerCallback(union sigval sv)
{
     std::cout << "TimerCallback: started" << std::endl;
    
    Scheduler* sched = static_cast<Scheduler*>(sv.sival_ptr);
    
    std::cout << "TimerCallback: about to pop..." << std::endl;
    
    task t;
    sched->m_waitable_queue.pop(&t);
    
    std::cout << "TimerCallback: popped! Executing..." << std::endl;

    t.first->Execute();
    
    task next;
    if (sched->m_waitable_queue.pop(&next, std::chrono::milliseconds(0)))
    {
        const auto nextTime = next.second;
        sched->m_waitable_queue.push(next);
        sched->SetTimer(nextTime);
    }
}

} // namespace ilrd