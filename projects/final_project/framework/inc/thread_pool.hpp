/*****************************************************************************
 * Exercise:    ThreadPool
 * Date:        03/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Shiran Swisa
 * Status:      Approved
 *****************************************************************************/

#ifndef _ILRD_THREADPOOL
#define _ILRD_THREADPOOL

#include <cstddef>              // std::size_t
#include <vector>               // std::vector
#include <memory>               // std::shared_ptr
#include <atomic>               // std::atomic
#include <mutex>                // std::mutex
#include <condition_variable>   // std::condition_variable
#include <thread>               // std::thread
#include <future>               // std::future

#include "waitablequeue.hpp"    // ilrd::WaitableQueue
#include "pq.hpp"               // ilrd::PriorityQueue

namespace ilrd
{

class ThreadPool
{
public:
    enum Priority
    {
        LOW = 1,
        MEDIUM = 2,
        HIGH = 3
    };

    class ITask
    {
    public:
        ITask() = default;
        virtual ~ITask() = default;
        virtual void Execute() = 0;

        ITask(const ITask&) = delete;
        ITask& operator=(const ITask&) = delete;
    };

    using TaskPtr = std::shared_ptr<ITask>;
    using Future = std::shared_future<void>;

    explicit ThreadPool(std::size_t numThreads = std::thread::hardware_concurrency());
    ~ThreadPool();

    Future Add(TaskPtr task, Priority priority = MEDIUM);

    void Run();
    void Pause();
    void Stop();
    void SetNumOfThreads(std::size_t numThreads);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    class TaskWrapper;
    class Worker;

    typedef std::shared_ptr<TaskWrapper> WrappedTaskPtr;
    typedef std::unique_ptr<Worker> WorkerPtr;

    struct ComparePriority
    {
        bool operator()(const WrappedTaskPtr& lhs, const WrappedTaskPtr& rhs) const;
    };

    
    std::vector<WorkerPtr> m_workers;

    using TaskPQ =  PriorityQueue<WrappedTaskPtr, std::vector<WrappedTaskPtr>, ComparePriority>;
    using TaskQueue = WaitableQueue<WrappedTaskPtr, TaskPQ>;
    TaskQueue m_taskQueue;
    
    std::atomic<bool> m_isRunning;
    std::atomic<bool> m_isStopped;
    
    std::mutex m_pauseMutex;
    std::condition_variable m_pauseCond;

    
    void CreateWorkers(std::size_t count);
    void SendPoisonPills(std::size_t count);

}; // class ThreadPool


class ThreadPool::TaskWrapper
{
public:
    // Regular task
    TaskWrapper(TaskPtr task, Priority priority);
    
    // Poison pill (signals worker to stop)
    explicit TaskWrapper(Priority priority);

    void Run();
    Priority GetPriority() const;
    bool IsPoisonPill() const;
    Future GetFuture();

private:
    TaskPtr m_task;
    Priority m_priority;
    std::promise<void> m_promise;
    bool m_isPoisonPill;
};

inline bool ThreadPool::ComparePriority::operator()(
    const WrappedTaskPtr& lhs,
    const WrappedTaskPtr& rhs) const
{
    return lhs->GetPriority() < rhs->GetPriority();
}

} // namespace ilrd

#endif // _ILRD_THREADPOOL