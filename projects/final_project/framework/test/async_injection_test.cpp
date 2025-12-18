#include <iostream>            // std::cout
#include <atomic>              // std::atomic
#include <thread>              // std::this_thread

#include "async_injection.hpp"

using namespace ilrd;

static void TestBasicAsyncInjection();

int main()
{
    TestBasicAsyncInjection();

    return 0;
}

static void TestBasicAsyncInjection()
{
    std::cout << "=== Test: Basic AsyncInjection ===" << std::endl;
    
    std::atomic<int> counter(0);
    
    new AsyncInjection(
        [&counter]() -> bool
        {
            ++counter;
            std::cout << "Tick " << counter << std::endl;
            
            return counter < 5;
        },
        std::chrono::milliseconds(500)
    );
    
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    
    if (counter == 5)
    {
        std::cout << "PASSED - counter reached 5" << std::endl;
    }
    else
    {
        std::cout << "FAILED - counter is " << counter << std::endl;
    }
}