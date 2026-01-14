#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

static std::mutex s_mutex;
static std::condition_variable s_cv;
static std::size_t s_threshold = 0;
static std::size_t s_count = 0;

void BarrierInit(std::size_t totalThreadsInSystem)
{
    s_threshold = totalThreadsInSystem;
    s_count = 0;
}

void BarrierLock()
{
    std::unique_lock<std::mutex> lock(s_mutex);

    ++s_count;

    if(s_threshold == s_count)
    {
        s_cv.notify_all();
    }
    else
    {
        s_cv.wait(lock,[] { return s_threshold == s_count;});
    }
}

void Worker(int id)
{
    std::cout << "Thread " << id << " arrived\n";
    BarrierLock();
    std::cout << "Thread " << id << " released\n";
}

int main()
{
    BarrierInit(3);
    
    std::vector<std::thread> threads;
    
    for (int i = 1; i <= 3; ++i)
    {
        threads.emplace_back(Worker, i);
    }
    
    for (auto& t : threads)
    {
        t.join();
    }
    
    return 0;

}
