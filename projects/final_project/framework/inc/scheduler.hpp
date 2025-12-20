/*****************************************************************************
 * Exercise:    Scheduler
 * Date:        16/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Dana Chesner
 * Status:      Approved
 *****************************************************************************/

#ifndef __ILRD_SCHEDLER__
#define __ILRD_SCHEDLER__

#include <sys/types.h>         // timer_t

#include <memory>              // std::shared_ptr
#include <chrono>              // std::chrono

#include "handleton.hpp"       // Handleton
#include "waitablequeue.hpp"   // WaitableQueue
#include "pq.hpp"              // PriorityQueue

namespace ilrd
{
class Scheduler
{
public:
    class ISTask // interface 
    {
    public:
        virtual ~ISTask() = default;
        virtual void Execute() = 0;    
    };

    using Duration  = std::chrono::milliseconds;
    using TimePoint = std::chrono::steady_clock::time_point;

    void Add(std::shared_ptr<ISTask> task, const Duration delay);
    
    Scheduler(const Scheduler& other) = delete;
    Scheduler& operator=(const Scheduler& other) = delete; 
    Scheduler(Scheduler&& other) = delete;
    Scheduler&& operator=(Scheduler&& other) = delete;    

private:

    //Tal Change 15:35
    using task = std::pair<std::shared_ptr<ISTask>, TimePoint>;
    
    friend class Handleton<Scheduler>;
    explicit Scheduler();
    ~Scheduler() noexcept;  

    struct TaskComparator 
    {
        bool operator()(const task& a, const task& b) const
        {
            return a.second > b.second;
        }
    };
    
    WaitableQueue<task, PriorityQueue<task,std::vector<task>, TaskComparator>> m_waitable_queue;
    timer_t m_timer;
    std::mutex m_mutex;
    static void TimerCallback(union sigval sv);
    void SetTimer(TimePoint exec_time);
    

}; // class Scheduler
}; // ilrd

#endif //__ILRD_SCHEDLER__

