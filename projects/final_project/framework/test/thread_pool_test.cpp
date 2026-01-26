/*******************************************************************************
 * Exercise: Semaphore Test
 * Date: 21/01/2026
 * Developer: Tal Hindi
 * Reviewer: 
 * Status: Testing
 ******************************************************************************/

#include <iostream>
#include <semaphore>
#include <thread>
#include <climits>

int main()
{
    std::cout << "Testing std::counting_semaphore...\n";
    
    // Simple semaphore test
    std::counting_semaphore<INT_MAX> sem(0);
    
    std::thread t([&sem]() {
        std::cout << "Thread: waiting on semaphore...\n";
        sem.acquire();
        std::cout << "Thread: acquired semaphore!\n";
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::cout << "Main: releasing semaphore...\n";
    sem.release();
    
    t.join();
    
    std::cout << "SUCCESS: std::counting_semaphore works!\n";
    
    return 0;
}