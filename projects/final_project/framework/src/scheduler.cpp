/*****************************************************************************
 * Exercise:    Scheduler
 * Date:        16/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Dana Chesner
 * Status:      Approved
 *****************************************************************************/

#include <signal.h>    // sigevent
#include <time.h>      // timer_create
#include <cstring>     // memset
#include <iostream>    // cout

#include "scheduler.hpp"

namespace ilrd
{

Scheduler::Scheduler()
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
    std::lock_guard<std::mutex> lock(m_mutex);

    TimePoint exec_time = std::chrono::steady_clock::now() + delay;
    m_waitable_queue.push({task, exec_time});
    SetTimer(exec_time);
}

void Scheduler::SetTimer(TimePoint exec_time)
{
    auto now = std::chrono::steady_clock::now();
    auto delay = exec_time - now;
    
    if (delay.count() <= 0)
    {
        delay = std::chrono::nanoseconds(1);
    }
    
    auto delay_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(delay);
    struct itimerspec its;
    std::memset(&its, 0, sizeof(its));
    
    its.it_value.tv_sec = delay.count() / 1000000000;
    its.it_value.tv_nsec = delay.count() % 1000000000;
    

    if (timer_settime(m_timer, 0, &its, nullptr) == -1)
    {
        throw std::runtime_error("Failed to set timer");
    }
}

void Scheduler::TimerCallback(union sigval sv)
{
    Scheduler* sched = static_cast<Scheduler*>(sv.sival_ptr);
    
    task currentTask;
    std::unique_lock<std::mutex> lock(sched->m_mutex);
    
    if (!sched->m_waitable_queue.pop(&currentTask, std::chrono::milliseconds(0)))
    {
        return;
    }
    
    if (currentTask.second > std::chrono::steady_clock::now())
    {
        sched->m_waitable_queue.push(currentTask);
        sched->SetTimer(currentTask.second);
        return;
    }
    
    lock.unlock();
    
    currentTask.first->Execute();
    
    lock.lock();
    
    task nextTask;
    if (sched->m_waitable_queue.pop(&nextTask, std::chrono::milliseconds(0)))
    {
        sched->m_waitable_queue.push(nextTask);
        sched->SetTimer(nextTask.second);
    }
}

} // namespace ilrd