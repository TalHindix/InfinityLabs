/*******************************************************************************
 * Exercise: ThreadPool
 * Date: 30/12/2024
 * Developer: Tal Hindi
 * Reviewer:
 * Status: In Progress
 ******************************************************************************/

#include <stdexcept> // std::runtime_error

#include "thread_pool.hpp"

namespace ilrd
{

thread_local bool t_isAlive = true;

// ======================= WorkerThread =======================================

class ThreadPool::WorkerThread
{
public:
    explicit WorkerThread(ThreadPool& pool)
        : m_pool(pool)
        , m_thread(&WorkerThread::Run, this)
    {}

    ~WorkerThread()
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

    WorkerThread(const WorkerThread&) = delete;
    WorkerThread& operator=(const WorkerThread&) = delete;

private:
    void Run()
    {
        while (t_isAlive)
        {
            TaskWithPriority task;
            m_pool.m_taskQueue.pop(&task);
            task.m_task->Execute();
        }

        m_pool.m_deadWorkers.push(this);
    }

    ThreadPool& m_pool;
    std::thread m_thread;
};

// ======================= BadApple ===========================================

class ThreadPool::BadApple : public ITask
{
public:
    void Execute() override
    {
        t_isAlive = false;
    }
};

// ======================= PauseTask ==========================================

class ThreadPool::PauseTask : public ITask
{
public:
    explicit PauseTask(sem_t* sem) : m_sem(sem) {}

    void Execute() override
    {
        sem_wait(m_sem);
    }

private:
    sem_t* m_sem;
};

// ======================= ThreadPool =========================================

ThreadPool::ThreadPool(std::size_t numThreads)
    : m_isRunning(false)
    , m_isStopped(false)
{
    sem_init(&m_pauseSem, 0, 0);
    SetNumOfThreads(numThreads);
}

ThreadPool::~ThreadPool()
{
    Stop();
    sem_destroy(&m_pauseSem);
}

void ThreadPool::Add(std::shared_ptr<ITask> task, Priority priority)
{
    if (m_isStopped.load())
    {
        throw std::runtime_error("Cannot add task: pool is stopped");
    }

    m_taskQueue.push({task, priority});
}

void ThreadPool::Run()
{
    if (m_isStopped.load() || m_isRunning.load())
    {
        return;
    }

    m_isRunning.store(true);

    for (std::size_t i = 0; i < m_workers.size(); ++i)
    {
        sem_post(&m_pauseSem);
    }
}

void ThreadPool::Pause()
{
    if (!m_isRunning.load() || m_isStopped.load())
    {
        return;
    }

    m_isRunning.store(false);

    for (std::size_t i = 0; i < m_workers.size(); ++i)
    {
        m_taskQueue.push({std::make_shared<PauseTask>(&m_pauseSem), FIRST});
    }
}

void ThreadPool::Stop()
{
    if (m_isStopped.exchange(true))
    {
        return;
    }

    m_isRunning.store(false);

    Shrink(m_workers.size());
}

void ThreadPool::SetNumOfThreads(std::size_t numThreads)
{
    if (m_isStopped.load())
    {
        throw std::runtime_error("Cannot resize: pool is stopped");
    }

    std::size_t currentThreads = m_workers.size();

    if (numThreads > currentThreads)
    {
        Grow(numThreads - currentThreads);
    }
    else if (numThreads < currentThreads)
    {
        Shrink(currentThreads - numThreads);
    }
}

void ThreadPool::Grow(std::size_t toAdd)
{
    for (std::size_t i = 0; i < toAdd; ++i)
    {
        m_workers.push_back(std::make_unique<WorkerThread>(*this));
    }

    if (!m_isRunning.load())
    {
        for (std::size_t i = 0; i < toAdd; ++i)
        {
            m_taskQueue.push({std::make_shared<PauseTask>(&m_pauseSem), FIRST});
        }
    }
}

void ThreadPool::Shrink(std::size_t toRemove)
{
    for (std::size_t i = 0; i < toRemove; ++i)
    {
        m_taskQueue.push({std::make_shared<BadApple>(), FIRST});
    }

    if (!m_isRunning.load())
    {
        for (std::size_t i = 0; i < toRemove; ++i)
        {
            sem_post(&m_pauseSem);
        }
    }

    for (std::size_t i = 0; i < toRemove; ++i)
    {
        WorkerThread* dead;
        m_deadWorkers.pop(&dead);

        auto it = std::find_if(m_workers.begin(), m_workers.end(),
            [dead](const std::unique_ptr<WorkerThread>& w)
            {
                return w.get() == dead;
            });

        if (it != m_workers.end())
        {
            m_workers.erase(it);
        }
    }
}

} // namespace ilrd