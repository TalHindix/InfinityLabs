#ifndef __ILRD_THREAD_POOL__
#define __ILRD_THREAD_POOL__

#include <cstddef> // std::size_t
#include <utility> // std::pair
#include <vector> // std::vector
#include <thread> // std::thread
#include <condition_variable> // std::condition_variable
#include <mutex> // std::mutex
#include <memory> // std::unique_ptr
#include <atomic> // std::atomic
#include <unordered_map>
#include <semaphore>
#include <climits> // INT_MAX


#include "waitablequeue.hpp"
#include "pq.hpp"

namespace ilrd
{
class ThreadPool
{   
public:

    using Semaphore = std::counting_semaphore<INT_MAX>;
    
    ThreadPool(std::size_t num_threads = std::thread::hardware_concurrency());
    ~ThreadPool() noexcept;
    enum e_Priority
    {
        LOW = 1,
        MEDIUM = 2,
        HIGH = 3
    };

    class ITPTask;

    using task_pair = std::pair<std::shared_ptr<ITPTask>, int>;
    struct Comp
    {
        bool operator()(const task_pair& a, const task_pair& b) const
        {
            return a.second < b.second;
        }
    };

    //adding tasks to queue
    void Add(std::shared_ptr<ITPTask>, e_Priority priority = MEDIUM) noexcept;
    //open a barrier
    void Run() noexcept;
    //close the barrier, ending running tasks
    void Pause() noexcept;
    // pause, SetNumOfThreads to 0, empty waiting queue
    void Stop() noexcept; 
    void SetNumOfThreads(std::size_t new_num_threads); // can throw std::bad_alloc  

    ThreadPool& operator=(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool&& other) = delete;
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool(const ThreadPool&& other) = delete;    
    
    class ITPTask
    {
    public:
        virtual ~ITPTask() = 0;
        virtual void Execute() = 0;
    };

private:
    class WorkerThread
    {
    public:
        WorkerThread(ThreadPool& thread_pool);
        ~WorkerThread() noexcept;
        void RunThread();
        WorkerThread(const WorkerThread& other) = delete;
        WorkerThread& operator=(const WorkerThread& other) noexcept = delete;
        WorkerThread(WorkerThread&& other) = delete;
        WorkerThread& operator=(WorkerThread&& other) noexcept = delete;

         class BadAppleTaskPause : public ThreadPool::ITPTask
        {
        public:
            friend class ThreadPool;
            BadAppleTaskPause(ThreadPool& thread_pool);
            void Execute() override;
        private:
            ThreadPool& m_thread_pool;
        }; // class BadAppleTaskPause	

        class BadAppleTaskStop : public ThreadPool::ITPTask
        {
        public:
            friend class ThreadPool;
            BadAppleTaskStop(ThreadPool& thread_pool);
            void Execute() override;
        private:
            ThreadPool& m_thread_pool;
        }; // class BadAppleTaskStop

    private:    
        friend class ThreadPool;

        inline static thread_local bool m_is_alive = true;	

        ThreadPool& m_thread_pool;
        std::thread m_thread;
        
    }; //class WorkerThread

    const int m_bad_apple_pause_priority = e_Priority::HIGH + 1;
    const int m_bad_apple_stop_priority = e_Priority::LOW - 1;


private:
    WaitableQueue<WorkerThread*> m_threads_to_remove;
    size_t m_num_threads;
    bool m_is_paused;
    WaitableQueue<task_pair, PriorityQueue<task_pair, std::vector<task_pair>, Comp>> m_tasks;
    Semaphore m_sem_pause;


    void StretchThreads(size_t new_num_threads);
    void ShrinkThreads(size_t new_num_threads);

    

}; // class ThreadPool
} // ilrd

#endif //__ILRD_THREAD_POOL__
