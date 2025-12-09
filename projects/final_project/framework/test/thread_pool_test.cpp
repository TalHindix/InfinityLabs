/**************************************
Check with Yuval tests , passed/ 
**************************************/

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RED "\033[1;31m"


#include <iostream> //std::cout 
#include <atomic> // std::atomic 

#include "thread_pool.hpp"

namespace ilrd
{
class TPTaskPrint : public ThreadPool::ITask
{
public:
    TPTaskPrint(std::string msg) : m_msg(msg) {}

    ~TPTaskPrint() = default;

    void Execute() override
    {
        s_count++;
        std::cout << m_msg << std::endl;
    }

    static std::atomic<size_t> s_count;
private:
    std::string m_msg;

}; //class TPTaskPrint

std::atomic<size_t> TPTaskPrint::s_count = 0;
} // ilrd

using namespace ilrd;

template <typename T>
static void PrintValidity(T result, T expected, const std::string& name);
static void SimpleFlowTest();
static void DifferentNumOfThreadsAndTasksTest();
static void ResizeThreadVectorTest();
static void PauseTest();
static void ShrinkThreadVectorTest();
static void PauseThenRunTest();
static void PauseStopAddTest();
static void RunThenAddTest();
static void AddRunThenAddTest();
static void PriorityTest();
static void PauseThenShrinkTest();


int main()
{
    SimpleFlowTest();
    DifferentNumOfThreadsAndTasksTest();
    ResizeThreadVectorTest();
    PauseTest();
    ShrinkThreadVectorTest();
    PauseThenRunTest();
    PauseStopAddTest();
    RunThenAddTest();
    AddRunThenAddTest();
    PriorityTest();
    PauseThenShrinkTest();

    return 0;
}

static void SimpleFlowTest()
{
    std::cout << "======= TestSimpleFlow =======" << std::endl;
    ThreadPool tp(1);
    for (size_t i = 0; i < 10; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i+1)), ThreadPool::LOW);
    }
    tp.Run();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));    
    PrintValidity<size_t>(TPTaskPrint::s_count, 10, "Run 10 Tasks Test");
}

static void DifferentNumOfThreadsAndTasksTest()
{
    std::cout << "======= TestDifferentNumOfThreadsAndTasks =======" << std::endl;
    
    {
        ThreadPool tp(2);
        for (size_t i = 0; i < 5; ++i)
        {
            tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
        }
        TPTaskPrint::s_count = 0;
        tp.Run();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));    
        PrintValidity<size_t>(TPTaskPrint::s_count, 5, "Run 5 Tasks with 2 threads");
    }

    //===============================
    
    {
        ThreadPool tp(5);
        for (size_t i = 0; i < 2; ++i)
        {
            tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
        }
        TPTaskPrint::s_count = 0;
        tp.Run();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));    
        PrintValidity<size_t>(TPTaskPrint::s_count, 2, "Run 2 Tasks with 5 threads");
    }

    {
        ThreadPool tp(5);
        for (size_t i = 0; i < 5; ++i)
        {
            tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
        }
        TPTaskPrint::s_count = 0;
        tp.Run();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));    
        PrintValidity<size_t>(TPTaskPrint::s_count, 5, "Run 5 Tasks with 5 threads");
    }

    {
        ThreadPool tp(2);
        
        TPTaskPrint::s_count = 0;
        tp.Run();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));    
        PrintValidity<size_t>(TPTaskPrint::s_count, 0, "Run 0 Tasks with 2 threads");
    }

    {
        ThreadPool tp(0);
        for (size_t i = 0; i < 3; ++i)
        {
            tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
        }
        TPTaskPrint::s_count = 0;
        tp.Run();
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
        tp.Pause();
        tp.Stop();   
        PrintValidity<size_t>(TPTaskPrint::s_count, 0, "Run 3 Tasks with 0 threads");
    }
}

static void ResizeThreadVectorTest()
{
    std::cout << "======= TestResizeThreadVector =======" << std::endl;
    ThreadPool tp(2);
    for (size_t i = 0; i < 5; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
    }
    TPTaskPrint::s_count = 0;
    tp.Run();
    tp.SetNumOfThreads(5);
    PrintValidity<size_t>(true, true, "SetNumOfThreads from 2 to 5");
    std::this_thread::sleep_for(std::chrono::milliseconds(1));    
    PrintValidity<size_t>(TPTaskPrint::s_count, 5, "Run 5 Tasks with 2 threads then 5 threads");
}

static void ShrinkThreadVectorTest()
{
    std::cout << "======= TestShrinkThreadVector =======" << std::endl;
    ThreadPool tp(6);
    for (size_t i = 0; i < 5; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
    }
    TPTaskPrint::s_count = 0;
    tp.Run();
    tp.SetNumOfThreads(2);
    PrintValidity<size_t>(true, true, "SetNumOfThreads from 5 to 2");
    std::this_thread::sleep_for(std::chrono::milliseconds(1));    
    PrintValidity<size_t>(TPTaskPrint::s_count, 5, "Run 5 Tasks with 5 threads then 2 threads");
}

static void PauseTest()
{
    std::cout << "======= TestResizeThreadVector =======" << std::endl;
    ThreadPool tp(2);
    for (size_t i = 0; i < 5; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
    }
    TPTaskPrint::s_count = 0;
    tp.Run();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));    
    tp.Pause();
    std::cout << "pause..." << std::endl;
    for (size_t i = 0; i < 2; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("shouldn't print!!! - Task" + std::to_string(i)), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(300));    
    PrintValidity<size_t>(TPTaskPrint::s_count, 5, "7 tasks with 5 threads, paused on 2 => print 5");
}

static void PauseThenRunTest()
{
    std::cout << "======= TestPauseThenRunTest =======" << std::endl;
    ThreadPool tp(2);
    for (size_t i = 0; i < 5; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
    }
    TPTaskPrint::s_count = 0;
    tp.Run();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));    
    tp.Pause();
    std::cout << "pause..." << std::endl;
    for (size_t i = 0; i < 2; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
    }

    tp.Run();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));    
    PrintValidity<size_t>(TPTaskPrint::s_count, 7, "7 tasks with 5 threads, paused on 2 then Run => print 7");
}

static void PauseStopAddTest()
{
    std::cout << "======= TestPauseStopAddTest =======" << std::endl;
    ThreadPool tp(2);
    for (size_t i = 0; i < 5; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
    }
    TPTaskPrint::s_count = 0;
    tp.Run();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));    
    tp.Pause();
    std::cout << "pause..." << std::endl;
    tp.Stop();
    PrintValidity<size_t>(TPTaskPrint::s_count, 5, "5 tasks with 2 threads, paused and stop");
    // for (size_t i = 0; i < 2; ++i)
    // {
    //     tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
    // }
    // TPTaskPrint::s_count = 0;
    //tp.SetNumOfThreads(2);
}

static void RunThenAddTest()
{
    std::cout << "======= TestRunThenAddTest =======" << std::endl;
    ThreadPool tp(2);
    TPTaskPrint::s_count = 0;
    tp.Run();
    for (size_t i = 0; i < 5; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));    
    
    PrintValidity<size_t>(TPTaskPrint::s_count, 5, "5 tasks with 2 threads, run then add");
}

static void AddRunThenAddTest()
{
    std::cout << "======= TestAddRunThenAddTest =======" << std::endl;
    ThreadPool tp(2);
    TPTaskPrint::s_count = 0;
    for (size_t i = 0; i < 5; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
    }
    tp.Run();
    for (size_t i = 0; i < 5; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));    
    
    PrintValidity<size_t>(TPTaskPrint::s_count, 10, "10 tasks with 2 threads, add run then add");
}

static void PriorityTest()
{
    std::cout << "======= PriorityTest =======" << std::endl;
    ThreadPool tp(5);
    size_t i = 0;

    TPTaskPrint::s_count = 0;
    for (; i < 10; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("MEDIUM - Task" + std::to_string(i)), ThreadPool::MEDIUM);
    }

    for (; i < 20; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("LOW - Task" + std::to_string(i)), ThreadPool::LOW);
    }

    //tp.Run();
    for (; i < 30; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("HIGH - Task" + std::to_string(i)), ThreadPool::HIGH);
    }  

    tp.Pause();
    std::cout << "pause..." << std::endl;
    tp.Run();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));    

    PrintValidity<size_t>(TPTaskPrint::s_count, 30, "30 priority test");
}

static void PauseThenShrinkTest()
{
    std::cout << "======= PauseThenShrinkTest =======" << std::endl;
    ThreadPool tp(100);
    for (size_t i = 0; i < 100; ++i)
    {
        tp.Add(std::make_shared<TPTaskPrint>("Task" + std::to_string(i)), ThreadPool::LOW);
    }
    TPTaskPrint::s_count = 0;
    tp.Run();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    tp.Pause();
    std::cout << "pause..." << std::endl;

    tp.SetNumOfThreads(10);
    std::cout << "after shrink..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    tp.Run();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));    
    PrintValidity<size_t>(TPTaskPrint::s_count, 100, "100 tasks with 100 threads then 10 thread");
}

template <typename T>
static void PrintValidity(T result, T expected, const std::string& name)
{
    if (result == expected)
    {
        std::cout << COLOR_GREEN "GOOD " COLOR_RESET << name << std::endl;
    }
    else
    {
        std::cout << COLOR_RED "BAD " COLOR_RESET << name << std::endl;
        std::cout << "Expected: " << expected
                  << "\nGot:      " << result << "\n";
    }
}
