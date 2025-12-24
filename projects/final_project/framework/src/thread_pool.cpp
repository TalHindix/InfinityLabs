/*****************************************************************************
 Exercise:    ThreadPool
 Date:        03/12/2025
 Developer:   Tal Hindi
 Reviewer:    Shiran Swisa
 Status:      Approved
 *****************************************************************************/

#include <stdexcept>    // std::runtime_error
#include <chrono>       // std::chrono::milliseconds

#include "thread_pool.hpp"
#include "logger.hpp"

namespace ilrd
{

static const int POISON_PILL_PRIORITY = ThreadPool::HIGH + 1;
static const std::chrono::milliseconds POP_TIMEOUT(100);


ThreadPool::TaskWrapper::TaskWrapper(TaskPtr task, Priority priority)
    : m_task(task)
    , m_priority(priority)
    , m_isPoisonPill(false)
{
}

ThreadPool::TaskWrapper::TaskWrapper(Priority priority)
    : m_priority(priority), m_isPoisonPill(true)
{
}

void ThreadPool::TaskWrapper::Run()
{
    if (m_isPoisonPill)
    {
        return;
    }

    try
    {
        if (m_task)
        {
            m_task->Execute();
        }
        m_promise.set_value();
    }
    catch (...)
    {
        m_promise.set_exception(std::current_exception());
    }
}

ThreadPool::Priority ThreadPool::TaskWrapper::GetPriority() const
{
    return m_priority;
}

bool ThreadPool::TaskWrapper::IsPoisonPill() const
{
    return m_isPoisonPill;
}

ThreadPool::Future ThreadPool::TaskWrapper::GetFuture()
{
    return m_promise.get_future().share();
}

class ThreadPool::Worker
{
public:
    Worker(TaskQueue& taskQueue,
           std::atomic<bool>& isRunning,
           std::atomic<bool>& isStopped,
           std::mutex& pauseMutex,
           std::condition_variable& pauseCond);
    ~Worker();

    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;

private:
    void WorkLoop();
    void WaitWhilePaused();

    TaskQueue& m_taskQueue;
    std::atomic<bool>& m_isRunning;
    std::atomic<bool>& m_isStopped;
    std::mutex& m_pauseMutex;
    std::condition_variable& m_pauseCond;
    std::thread m_thread;
};


ThreadPool::Worker::Worker(TaskQueue& taskQueue,
                           std::atomic<bool>& isRunning,
                           std::atomic<bool>& isStopped,
                           std::mutex& pauseMutex,
                           std::condition_variable& pauseCond)
    : m_taskQueue(taskQueue)
    , m_isRunning(isRunning)
    , m_isStopped(isStopped)
    , m_pauseMutex(pauseMutex)
    , m_pauseCond(pauseCond)
    , m_thread(&Worker::WorkLoop, this)
{
}

ThreadPool::Worker::~Worker()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void ThreadPool::Worker::WorkLoop()
{
    
    while (!m_isStopped.load())
    {
        WaitWhilePaused();

        if (m_isStopped.load())
        {
            break;
        }

        WrappedTaskPtr task;
        bool hasTask = m_taskQueue.pop(&task, POP_TIMEOUT);

        if (hasTask)
        {
            if (task->IsPoisonPill())
            {
                break;
            }

            if (m_isRunning.load())
            {
                task->Run();
            }
        }
    }
    
}

void ThreadPool::Worker::WaitWhilePaused()
{
    std::unique_lock<std::mutex> lock(m_pauseMutex);

    while (!m_isRunning.load() && !m_isStopped.load())
    {
        m_pauseCond.wait(lock);
    }
}

ThreadPool::ThreadPool(std::size_t numThreads)
    : m_isRunning(false)
    , m_isStopped(false)
{
    THREADPOOL_LOG(Logger::DEBUGING, "Ctor Started..");
    CreateWorkers(numThreads);
}

ThreadPool::~ThreadPool()
{
    THREADPOOL_LOG(Logger::DEBUGING, "Dtor Started..");
    
    if (!m_isStopped.load())
    {
        Stop();
    }
    m_workers.clear();
}

ThreadPool::Future ThreadPool::Add(TaskPtr task, Priority priority)
{
    if (m_isStopped.load())
    {
        THREADPOOL_LOG(Logger::ERROR, "Cannot add task to stopped ThreadPool");
        throw std::runtime_error("Cannot add task to stopped ThreadPool");
    }

    WrappedTaskPtr wrappedTask(new TaskWrapper(task, priority));
    Future future = wrappedTask->GetFuture();

    m_taskQueue.push(wrappedTask);

    return future;
}

void ThreadPool::Run()
{
    THREADPOOL_LOG(Logger::DEBUGING, "Run()");
    m_isRunning.store(true);
    m_pauseCond.notify_all();
}

void ThreadPool::Pause()
{
    THREADPOOL_LOG(Logger::DEBUGING, "Pause()");
    m_isRunning.store(false);
}

void ThreadPool::Stop()
{
    THREADPOOL_LOG(Logger::DEBUGING, "Stop()");
    m_isStopped.store(true);
    m_isRunning.store(false);
    SendPoisonPills(m_workers.size());
    m_pauseCond.notify_all();
}

void ThreadPool::SetNumOfThreads(std::size_t numThreads)
{
    if (m_isStopped.load())
    {
        THREADPOOL_LOG(Logger::ERROR, "Cannot resize stopped ThreadPool");
        throw std::runtime_error("Cannot change thread count on stopped ThreadPool");
    }

    std::size_t currentCount = m_workers.size();
    
    THREADPOOL_LOG(Logger::DEBUGING, "SetNumOfThreads: " + 
                   std::to_string(currentCount) + " -> " + 
                   std::to_string(numThreads));

    if (numThreads > currentCount)
    {
        CreateWorkers(numThreads - currentCount);
    }
    else if (numThreads < currentCount)
    {
        SendPoisonPills(currentCount - numThreads);
    }
}

void ThreadPool::CreateWorkers(std::size_t count)
{
    THREADPOOL_LOG(Logger::DEBUGING, "Create:"+std::to_string(count)+" workers");
    for (std::size_t i = 0; i < count; ++i)
    {
        m_workers.push_back(WorkerPtr(new Worker(
            m_taskQueue,
            m_isRunning,
            m_isStopped,
            m_pauseMutex,
            m_pauseCond
        )));
    }
}

void ThreadPool::SendPoisonPills(std::size_t count)
{
    Priority poisonPriority = static_cast<Priority>(POISON_PILL_PRIORITY);

    for (std::size_t i = 0; i < count; ++i)
    {
        WrappedTaskPtr pill(new TaskWrapper(poisonPriority));
        m_taskQueue.push(pill);
    }
}

} // namespace ilrd