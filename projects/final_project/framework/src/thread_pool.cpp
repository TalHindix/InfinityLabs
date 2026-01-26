#include "thread_pool.hpp"
#include "logger.hpp"

namespace ilrd
{

ThreadPool::ThreadPool(std::size_t num_threads) : m_num_threads(0), m_is_paused(true), m_sem_pause(0)
{
    LOG_DEBUG("[ThreadPool] ctor ENTER");    

    SetNumOfThreads(num_threads);

    LOG_DEBUG("[ThreadPool] ctor EXIT");    
}

ThreadPool::~ThreadPool() noexcept
{
    LOG_DEBUG("[ThreadPool] dtor ENTER");    
    Stop();
    LOG_DEBUG("[ThreadPool] dtor EXIT");    
}

void ThreadPool::Add(std::shared_ptr<ITPTask> task, e_Priority priority) noexcept
{
    LOG_DEBUG("[ThreadPool] Add ENTER");    
    m_tasks.push({std::move(task), priority});
    LOG_DEBUG("[ThreadPool] Add EXIT");    
}

void ThreadPool::Run() noexcept
{    
    LOG_DEBUG("[ThreadPool] Run ENTER");
    if(m_is_paused)
    {
        m_sem_pause.release(m_num_threads);
        m_is_paused = false;
    }
    LOG_DEBUG("[ThreadPool] Run EXIT");
}

void ThreadPool::Pause() noexcept
{
    LOG_DEBUG("[ThreadPool] Pause ENTER");
    if(!m_is_paused)
    {
        m_is_paused = true;
        for(size_t i = 0; i < m_num_threads; ++i)
        {
            Add(std::make_shared<WorkerThread::BadAppleTaskPause>(*this), static_cast<e_Priority>(m_bad_apple_pause_priority));
        }
    }
    LOG_DEBUG("[ThreadPool] Pause EXIT");
}

void ThreadPool::Stop() noexcept
{
    LOG_DEBUG("[ThreadPool] Stop ENTER");

    SetNumOfThreads(0);

    LOG_DEBUG("[ThreadPool] Stop EXIT");
}

void ThreadPool::SetNumOfThreads(std::size_t new_num_threads) 
{
    LOG_DEBUG("[ThreadPool] SetNumOfThreads ENTER");
    if(m_num_threads < new_num_threads)
    {
        StretchThreads(new_num_threads - m_num_threads);
    }
    else
    {
        ShrinkThreads(m_num_threads - new_num_threads);
    }
    
    m_num_threads = new_num_threads;
    LOG_DEBUG("[ThreadPool] SetNumOfThreads EXIT");
}

void ThreadPool::StretchThreads(size_t num_to_add)
{
    if(m_is_paused)
    {
        for(size_t i = 0; i < num_to_add; ++i)
        {
            Add(std::make_shared<WorkerThread::BadAppleTaskPause>(*this), static_cast<e_Priority>(m_bad_apple_pause_priority));
        }
    }
    for(size_t i = 0; i < num_to_add; ++i)
    {
        new WorkerThread(*this);
    }
}

void ThreadPool::ShrinkThreads(size_t num_to_remove)
{
     for(size_t i = 0; i < num_to_remove; ++i)
        {
            Add(std::make_shared<WorkerThread::BadAppleTaskStop>(*this), static_cast<e_Priority>(m_bad_apple_stop_priority));
        }
        
        if(m_is_paused)
        {
            m_sem_pause.release(num_to_remove);
        } 

        for(size_t i = 0; i < num_to_remove; i++)
        {
            WorkerThread* worker_thread;
            m_threads_to_remove.pop(&worker_thread);

            if(worker_thread->m_thread.joinable())
            {
                worker_thread->m_thread.join();
            }
            delete worker_thread;
        }
}

ThreadPool::ITPTask::~ITPTask()
{
    LOG_DEBUG("[ThreadPool::ITPTask] dtor");
}


//================WorkerThread===================
ThreadPool::WorkerThread::WorkerThread(ThreadPool& thread_pool) :   m_thread_pool(thread_pool),
                                                                    m_thread(&WorkerThread::RunThread, this)
                                                                   
{
    LOG_DEBUG("[ThreadPool::WorkerThread] ctor");
}

ThreadPool::WorkerThread::~WorkerThread()
{
    LOG_DEBUG("[ThreadPool::WorkerThread] ctor ENTER");
    LOG_DEBUG("[WorkerThread] ctor EXIT");
}

void ThreadPool::WorkerThread::RunThread()
{
    LOG_DEBUG("[ThreadPool::WorkerThread] RunThread ENTER");
    m_is_alive = true;

    while(m_is_alive)
    {
        ThreadPool::task_pair task;
        m_thread_pool.m_tasks.pop(&task);
        
        task.first->Execute();
    }

    m_thread_pool.m_threads_to_remove.push(this);
    LOG_DEBUG("[ThreadPool::WorkerThread] RunThread EXIT");
}
//====================================

ThreadPool::WorkerThread::BadAppleTaskPause::BadAppleTaskPause(ThreadPool& thread_pool) : m_thread_pool(thread_pool)
{

}

void ThreadPool::WorkerThread::BadAppleTaskPause::Execute()
{
    m_thread_pool.m_sem_pause.acquire();
}

ThreadPool::WorkerThread::BadAppleTaskStop::BadAppleTaskStop(ThreadPool& thread_pool) : m_thread_pool(thread_pool)
{

}

void ThreadPool::WorkerThread::BadAppleTaskStop::Execute()
{
    WorkerThread::m_is_alive = false;
}


} // ilrd
