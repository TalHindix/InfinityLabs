/*****************************************************************************
 * Exercise:    AsyncInjection
 * Date:        17/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#ifndef __ILRD_ASYNC_INJECTION__
#define __ILRD_ASYNC_INJECTION__

#include "scheduler.hpp"       // Scheduler::ISTask

namespace ilrd
{

class AsyncInjection
{
public:
    AsyncInjection(std::function<bool(void)> func, 
                   std::chrono::milliseconds interval);
    ~AsyncInjection() = default;

    AsyncInjection(const AsyncInjection&) = delete;
    AsyncInjection& operator=(const AsyncInjection&) = delete;

private:
    class AsyncTask : public Scheduler::ISTask
    {
    public:
        explicit AsyncTask(AsyncInjection& owner);
        void Execute() override;
    
    private:
        AsyncInjection& m_owner;
    };

    std::function<bool(void)> m_func;
    std::chrono::milliseconds m_interval;
    std::shared_ptr<Scheduler::ISTask> m_task;

    void Schedule();

}; // class AsyncInjection

} // namespace ilrd

#endif // __ILRD_ASYNC_INJECTION__