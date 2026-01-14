/*****************************************************************************
 * Exercise:    ThreadPool - Comprehensive Tests
 * Date:        29/12/2025
 * Developer:   Tal Hindi
 *****************************************************************************/

#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include "thread_pool.hpp"

namespace ilrd
{

class CountingTask : public ThreadPool::ITask
{
public:
    CountingTask(std::atomic<int>& counter) : m_counter(counter) {}
    
    void Execute() override
    {
        ++m_counter;
    }

private:
    std::atomic<int>& m_counter;
};

class SlowTask : public ThreadPool::ITask
{
public:
    SlowTask(std::atomic<int>& counter, int ms) : m_counter(counter), m_ms(ms) {}
    
    void Execute() override
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_ms));
        ++m_counter;
    }

private:
    std::atomic<int>& m_counter;
    int m_ms;
};

class PrintTask : public ThreadPool::ITask
{
public:
    PrintTask(const std::string& msg) : m_msg(msg) {}
    
    void Execute() override
    {
        std::cout << m_msg << std::endl;
    }

private:
    std::string m_msg;
};

} // namespace ilrd

using namespace ilrd;

static void Wait(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

static void PrintResult(bool passed, const std::string& testName)
{
    if (passed)
    {
        std::cout << "\033[1;32mPASS\033[0m - " << testName << std::endl;
    }
    else
    {
        std::cout << "\033[1;31mFAIL\033[0m - " << testName << std::endl;
    }
}

// ============================================================================
// Basic Tests
// ============================================================================

static void TestCreateDestroy()
{
    std::cout << "\n=== Test: Create and Destroy ===" << std::endl;
    
    {
        ThreadPool pool(4);
    }
    
    PrintResult(true, "Create and destroy without crash");
}

static void TestCreateZeroThreads()
{
    std::cout << "\n=== Test: Create Zero Threads ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(0);
        pool.Add(std::make_shared<CountingTask>(counter));
        pool.Run();
        Wait(50);
    }
    
    PrintResult(counter == 0, "Zero threads = no work done");
    std::cout << "  Counter: " << counter << " (expected: 0)" << std::endl;
}

static void TestAddAndRun()
{
    std::cout << "\n=== Test: Add and Run ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(2);
        
        for (int i = 0; i < 5; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        pool.Run();
        Wait(100);
    }
    
    PrintResult(counter == 5, "5 tasks executed");
    std::cout << "  Counter: " << counter << " (expected: 5)" << std::endl;
}

static void TestRunThenAdd()
{
    std::cout << "\n=== Test: Run then Add ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(2);
        
        pool.Run();
        
        for (int i = 0; i < 3; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        Wait(100);
    }
    
    PrintResult(counter == 3, "Tasks run after Run() called first");
    std::cout << "  Counter: " << counter << " (expected: 3)" << std::endl;
}

// ============================================================================
// Double/Multiple Calls Tests
// ============================================================================

static void TestDoubleRun()
{
    std::cout << "\n=== Test: Double Run ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(2);
        
        for (int i = 0; i < 5; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        pool.Run();
        pool.Run();
        pool.Run();
        
        Wait(100);
    }
    
    PrintResult(counter == 5, "Multiple Run() calls don't break");
    std::cout << "  Counter: " << counter << " (expected: 5)" << std::endl;
}

static void TestDoublePause()
{
    std::cout << "\n=== Test: Double Pause ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(2);
        
        for (int i = 0; i < 5; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        Wait(50);
        int afterPause = counter.load();
        
        pool.Run();
        Wait(100);
        int afterRun = counter.load();
        
        PrintResult(afterPause == 0 && afterRun == 5, "Tasks wait then run");
        std::cout << "  After Pause: " << afterPause << ", After Run: " << afterRun << std::endl;
    }
}

static void TestDoubleStop()
{
    std::cout << "\n=== Test: Double Stop ===" << std::endl;
    
    {
        ThreadPool pool(2);
        pool.Run();
        pool.Stop();
        pool.Stop();
        pool.Stop();
    }
    
    PrintResult(true, "Multiple Stop() calls don't crash");
}

static void TestRunPauseRunPause()
{
    std::cout << "\n=== Test: Run-Pause-Run-Pause ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(2);
        
        for (int i = 0; i < 10; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        pool.Run();
        Wait(30);
        pool.Pause();
        Wait(30);
        pool.Run();
        Wait(30);
        pool.Pause();
        Wait(30);
        pool.Run();
        Wait(100);
    }
    
    PrintResult(counter == 10, "Run-Pause cycles work");
    std::cout << "  Counter: " << counter << " (expected: 10)" << std::endl;
}

// ============================================================================
// Pause Tests
// ============================================================================

static void TestPause()
{
    std::cout << "\n=== Test: Pause ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(2);
        
        for (int i = 0; i < 5; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        Wait(100);
        
        int afterPause = counter.load();
        PrintResult(afterPause == 0, "No tasks run while paused");
        std::cout << "  Counter after pause: " << afterPause << " (expected: 0)" << std::endl;
    }
}

static void TestPauseThenRun()
{
    std::cout << "\n=== Test: Pause then Run ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(2);
        
        for (int i = 0; i < 5; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        Wait(50);
        int beforeRun = counter.load();
        
        pool.Run();
        Wait(100);
        int afterRun = counter.load();
        
        PrintResult(beforeRun == 0 && afterRun == 5, "Tasks wait for Run()");
        std::cout << "  Before Run: " << beforeRun << ", After Run: " << afterRun << std::endl;
    }
}

static void TestPauseDuringWork()
{
    std::cout << "\n=== Test: Pause During Work ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(2);
        
        for (int i = 0; i < 20; ++i)
        {
            pool.Add(std::make_shared<SlowTask>(counter, 20));
        }
        
        pool.Run();
        Wait(50);
        
        pool.Pause();
        Wait(100);
        
        pool.Run();
        Wait(500);
        int afterResume = counter.load();
        
        PrintResult(afterResume == 20, "Resume completes all tasks");
        std::cout << "  After Resume: " << afterResume << " (expected: 20)" << std::endl;
    }
}

// ============================================================================
// SetNumOfThreads Tests
// ============================================================================

static void TestIncreaseThreads()
{
    std::cout << "\n=== Test: Increase Threads ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(1);
        pool.Run();
        
        pool.SetNumOfThreads(4);
        
        for (int i = 0; i < 10; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        Wait(100);
    }
    
    PrintResult(counter == 10, "Works after increasing threads");
    std::cout << "  Counter: " << counter << " (expected: 10)" << std::endl;
}

static void TestDecreaseThreads()
{
    std::cout << "\n=== Test: Decrease Threads ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(4);
        pool.Run();
        Wait(20);
        
        pool.SetNumOfThreads(1);
        
        for (int i = 0; i < 5; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        pool.Run();
        Wait(200);
    }
    
    PrintResult(counter == 5, "Works after decreasing threads");
    std::cout << "  Counter: " << counter << " (expected: 5)" << std::endl;
}

static void TestSetNumToZero()
{
    std::cout << "\n=== Test: SetNumOfThreads to Zero ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(4);
        pool.Run();
        Wait(20);
        
        pool.SetNumOfThreads(0);
        
        for (int i = 0; i < 5; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        Wait(100);
        int afterZero = counter.load();
        
        pool.SetNumOfThreads(2);
        pool.Run();
        Wait(100);
        int afterRestore = counter.load();
        
        PrintResult(afterZero == 0 && afterRestore == 5, "Zero threads = no work, restore works");
        std::cout << "  After Zero: " << afterZero << ", After Restore: " << afterRestore << std::endl;
    }
}

static void TestSetNumSameValue()
{
    std::cout << "\n=== Test: SetNumOfThreads Same Value ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(4);
        pool.Run();
        
        pool.SetNumOfThreads(4);
        pool.SetNumOfThreads(4);
        pool.SetNumOfThreads(4);
        
        for (int i = 0; i < 5; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        Wait(100);
    }
    
    PrintResult(counter == 5, "Same value doesn't break");
    std::cout << "  Counter: " << counter << " (expected: 5)" << std::endl;
}

static void TestSetNumUpDown()
{
    std::cout << "\n=== Test: SetNumOfThreads Up and Down ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(2);
        pool.Run();
        
        pool.SetNumOfThreads(10);
        Wait(20);
        pool.Run();
        pool.SetNumOfThreads(1);
        Wait(20);
        pool.Run();
        pool.SetNumOfThreads(5);
        Wait(20);
        pool.Run();
        
        for (int i = 0; i < 20; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        Wait(300);
    }
    
    PrintResult(counter == 20, "Multiple resize operations work");
    std::cout << "  Counter: " << counter << " (expected: 20)" << std::endl;
}

static void TestSetNumWhilePaused()
{
    std::cout << "\n=== Test: SetNumOfThreads While Paused ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(4);
        
        pool.SetNumOfThreads(2);
        
        for (int i = 0; i < 5; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        Wait(50);
        int beforeRun = counter.load();
        
        pool.Run();
        Wait(100);
        int afterRun = counter.load();
        
        PrintResult(beforeRun == 0 && afterRun == 5, "Resize while paused works");
        std::cout << "  Before Run: " << beforeRun << ", After Run: " << afterRun << std::endl;
    }
}

// ============================================================================
// Stop Tests
// ============================================================================

static void TestStop()
{
    std::cout << "\n=== Test: Stop ===" << std::endl;
    
    bool exceptionThrown = false;
    
    {
        ThreadPool pool(2);
        pool.Run();
        pool.Stop();
        
        try
        {
            pool.Add(std::make_shared<PrintTask>("Should not print"));
        }
        catch (const std::runtime_error&)
        {
            exceptionThrown = true;
        }
    }
    
    PrintResult(exceptionThrown, "Cannot add after Stop()");
}

static void TestStopWithPendingTasks()
{
    std::cout << "\n=== Test: Stop With Pending Tasks ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(1);
        
        for (int i = 0; i < 100; ++i)
        {
            pool.Add(std::make_shared<SlowTask>(counter, 10));
        }
        
        pool.Run();
        Wait(50);
        pool.Stop();
    }
    
    std::cout << "  Completed: " << counter << " out of 100" << std::endl;
    PrintResult(counter < 100, "Stop terminates before all tasks complete");
}

static void TestStopWhilePaused()
{
    std::cout << "\n=== Test: Stop While Paused ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(2);
        
        for (int i = 0; i < 10; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        pool.Stop();
    }
    
    PrintResult(counter == 0, "Stop while paused doesn't execute tasks");
    std::cout << "  Counter: " << counter << " (expected: 0)" << std::endl;
}

static void TestSetNumAfterStop()
{
    std::cout << "\n=== Test: SetNumOfThreads After Stop ===" << std::endl;
    
    bool exceptionThrown = false;
    
    {
        ThreadPool pool(2);
        pool.Stop();
        
        try
        {
            pool.SetNumOfThreads(4);
        }
        catch (const std::runtime_error&)
        {
            exceptionThrown = true;
        }
    }
    
    PrintResult(exceptionThrown, "Cannot SetNumOfThreads after Stop()");
}

// ============================================================================
// Priority Tests
// ============================================================================

static void TestPriority()
{
    std::cout << "\n=== Test: Priority ===" << std::endl;
    
    std::vector<int> order;
    std::mutex orderMutex;
    
    class OrderTask : public ThreadPool::ITask
    {
    public:
        OrderTask(int id, std::vector<int>& order, std::mutex& mutex)
            : m_id(id), m_order(order), m_mutex(mutex) {}
        
        void Execute() override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_order.push_back(m_id);
        }
    
    private:
        int m_id;
        std::vector<int>& m_order;
        std::mutex& m_mutex;
    };
    
    {
        ThreadPool pool(1);
        
        pool.Add(std::make_shared<OrderTask>(1, order, orderMutex), ThreadPool::LOW);
        pool.Add(std::make_shared<OrderTask>(2, order, orderMutex), ThreadPool::LOW);
        pool.Add(std::make_shared<OrderTask>(3, order, orderMutex), ThreadPool::HIGH);
        pool.Add(std::make_shared<OrderTask>(4, order, orderMutex), ThreadPool::MEDIUM);
        pool.Add(std::make_shared<OrderTask>(5, order, orderMutex), ThreadPool::HIGH);
        
        pool.Run();
        Wait(100);
    }
    
    std::cout << "  Order: ";
    for (int id : order)
    {
        std::cout << id << " ";
    }
    std::cout << std::endl;
    
    bool highFirst = (order.size() >= 2 && (order[0] == 3 || order[0] == 5) && (order[1] == 3 || order[1] == 5));
    PrintResult(highFirst, "HIGH priority tasks execute first");
}

// ============================================================================
// Stress Tests
// ============================================================================

static void TestManyTasks()
{
    std::cout << "\n=== Test: Many Tasks ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(8);
        pool.Run();
        
        for (int i = 0; i < 1000; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        Wait(500);
    }
    
    PrintResult(counter == 1000, "1000 tasks executed");
    std::cout << "  Counter: " << counter << " (expected: 1000)" << std::endl;
}

static void TestManyThreads()
{
    std::cout << "\n=== Test: Many Threads ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(50);
        pool.Run();
        
        for (int i = 0; i < 100; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
        }
        
        Wait(200);
    }
    
    PrintResult(counter == 100, "50 threads handle 100 tasks");
    std::cout << "  Counter: " << counter << " (expected: 100)" << std::endl;
}

static void TestRapidResizing()
{
    std::cout << "\n=== Test: Rapid Resizing ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    {
        ThreadPool pool(4);
        pool.Run();
        
        for (int i = 0; i < 50; ++i)
        {
            pool.Add(std::make_shared<CountingTask>(counter));
            pool.SetNumOfThreads((i % 10) + 1);
            pool.Run();
        }
        
        Wait(500);
    }
    
    PrintResult(counter == 50, "Rapid resizing doesn't lose tasks");
    std::cout << "  Counter: " << counter << " (expected: 50)" << std::endl;
}

// ============================================================================
// Main
// ============================================================================

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "   ThreadPool Comprehensive Tests" << std::endl;
    std::cout << "========================================" << std::endl;

    std::atomic<int> counter(0);
    ThreadPool pool(2);
    pool.Run();
    pool.Add(std::make_shared<CountingTask>(counter));
    Wait(100);
    PrintResult(counter == 1, "50 threads handle 100 tasks");
    // // Basic
    // TestCreateDestroy();
    // TestCreateZeroThreads();
    // TestAddAndRun();
    // TestRunThenAdd();
    
    // // Double/Multiple Calls
    // TestDoubleRun();
    // TestDoublePause();
    // TestDoubleStop();
    // TestRunPauseRunPause();
    
    // // Pause
    // TestPause();
    // TestPauseThenRun();
    // TestPauseDuringWork();
    
    // // SetNumOfThreads
    // TestIncreaseThreads();
    // TestDecreaseThreads();
    // TestSetNumToZero();
    // TestSetNumSameValue();
    // TestSetNumUpDown();
    // TestSetNumWhilePaused();
    
    // // Stop
    // TestStop();
    // TestStopWithPendingTasks();
    // TestStopWhilePaused();
    // TestSetNumAfterStop();
    
    // // Priority
    // TestPriority();
    
    // // Stress
    // TestManyTasks();
    // TestManyThreads();
    // TestRapidResizing();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "         Tests Complete" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}