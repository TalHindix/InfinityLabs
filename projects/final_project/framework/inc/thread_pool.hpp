/*******************************************************************************
 * Exercise: ThreadPool
 * Date: 30/12/2024
 * Developer: Tal Hindi
 * Reviewer:
 * Status: In Progress
 ******************************************************************************/

#ifndef ILRD_THREADPOOL_
#define ILRD_THREADPOOL_

#include <cstddef>     // std::size_t
#include <memory>      // std::shared_ptr
#include <thread>      // std::thread
#include <atomic>      // std::atomic
#include <vector>      // std::vector
#include <algorithm>   // std::find_if
#include <semaphore.h> // sem_t

#include "waitablequeue.hpp" // WaitableQueue
#include "pq.hpp"            // PriorityQueue

namespace ilrd
{

class ThreadPool
{
public:
    enum Priority { LOW = 1, MEDIUM, HIGH, FIRST };

    class ITask
    {
    public:
        ITask() = default;
        virtual ~ITask() = default;
        virtual void Execute() = 0;

        ITask(const ITask&) = delete;
        ITask& operator=(const ITask&) = delete;
    };

    explicit ThreadPool(std::size_t numThreads = std::thread::hardware_concurrency());
    ~ThreadPool();

    void Add(std::shared_ptr<ITask> task, Priority priority = MEDIUM);
    void Run();
    void Pause();
    void Stop();
    void SetNumOfThreads(std::size_t numThreads);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    class WorkerThread;
    class BadApple;
    class PauseTask;

    struct TaskWithPriority
    {
        std::shared_ptr<ITask> m_task;
        Priority m_priority;
    };

    struct ComparePriority
    {
        bool operator()(const TaskWithPriority& lhs,
                        const TaskWithPriority& rhs) const
        {
            return lhs.m_priority < rhs.m_priority;
        }
    };

    using TaskPQ = PriorityQueue<TaskWithPriority,
                                  std::vector<TaskWithPriority>,
                                  ComparePriority>;
    using TaskQueue = WaitableQueue<TaskWithPriority, TaskPQ>;

    void Grow(std::size_t toAdd);
    void Shrink(std::size_t toRemove);

    TaskQueue m_taskQueue;
    std::vector<std::unique_ptr<WorkerThread>> m_workers;
    WaitableQueue<WorkerThread*> m_deadWorkers;
    sem_t m_pauseSem;
    std::atomic<bool> m_isRunning;
    std::atomic<bool> m_isStopped;
    std::atomic<std::size_t> m_numThreads;
};

} // namespace ilrd

#endif // ILRD_THREADPOOL_