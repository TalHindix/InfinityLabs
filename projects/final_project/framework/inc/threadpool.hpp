/*****************************************************************************
 * Exercise:    ThreadPool
 * Date:        1/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#ifndef __ILRD_THREAD_POOL__
#define __ILRD_THREAD_POOL__

namespace ilrd
{
class ThreadPool
{
    
public:

    ThreadPool(std::size_t num_threads = std::thread::hardware_concurency());
    ~ThreadPool();
    enum Priority
    {
        LOW
        MEDIUM
        HIGH
    } e_Priority;

    class ITPTask;
    void Add(std::shared_ptr<ITPTask>, e_Priority priority = MEDIUM);
    void Run();
    void Pause();
    void Stop();
    void SetNumOfThreads(size_t new_num_threads);
    
    ThreadPool& operator=(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool&& other) = delete;
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool(const ThreadPool&& other) = delete;    

private:

    std::vector<std::thread> m_threads;
    WaitableQueue m_tasks;
    bool m_is_running;
}; // class ThreaPool
} // ilrd

#endif //__ILRD_THREAD_POOL__




}