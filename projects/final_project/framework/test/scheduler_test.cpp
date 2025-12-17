/*****************************************************************************
 * Exercise:    Scheduler
 * Date:        16/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Dana Chesner
 * Status:      
 *****************************************************************************/


#include <iostream>     // std::cout
#include <chrono>       // std::chrono
#include <thread>       // std::this_thread

#include "scheduler.hpp" // Scheduler
#include "handleton.hpp" // Handleton

using namespace ilrd;

class PrintTask : public Scheduler::ISTask
{
public:
    PrintTask(const std::string& msg) : m_msg(msg) {}
    
    void Execute() override
    {
        std::cout << "Task executed: " << m_msg << std::endl;
    }

private:
    std::string m_msg;
};


class ChainTask : public Scheduler::ISTask
{
public:
    ChainTask(int id) : m_id(id) {}
    
    void Execute() override
    {
        std::cout << "ChainTask " << m_id << " executed" << std::endl;
        
        if (m_id < 3)
        {
            Scheduler* sched = Handleton<Scheduler>::GetInstance();
            sched->Add(std::make_shared<ChainTask>(m_id + 1),
                       std::chrono::milliseconds(500));
            std::cout << "ChainTask " << m_id << " added ChainTask " << m_id + 1 << std::endl;
        }
    }

private:
    int m_id;
};

void TestSingleTask()
{
    Scheduler* sched = Handleton<Scheduler>::GetInstance();
    
    std::cout << "Adding task..." << std::endl;
    
    sched->Add(std::make_shared<PrintTask>("Hello after 2 seconds!"),
              std::chrono::milliseconds(2000));
    
    std::cout << "Waiting..." << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    
    std::cout << "Test 1 - Done!" << std::endl;
}

void TestTwoTasksPriorityOrder()
{
    Scheduler* sched = Handleton<Scheduler>::GetInstance();
    
    std::cout << "Adding two tasks..." << std::endl;
    
    sched->Add(std::make_shared<PrintTask>("Task A (3 sec) !"),
              std::chrono::milliseconds(3000));

    sched->Add(std::make_shared<PrintTask>("Task B (1 sec) !"),
              std::chrono::milliseconds(1000));
    
    std::cout << "Expected order: B then A" << std::endl;
    std::cout << "Waiting for tasks..." << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        
    std::cout << "Test 2 - Done!" << std::endl;
}

void TestTaskAddingTask()
{
    std::cout << "\n Task Adding Another Task " << std::endl;
    
    Scheduler* sched = Handleton<Scheduler>::GetInstance();
    
    sched->Add(std::make_shared<ChainTask>(1),
               std::chrono::milliseconds(500));
    
    std::cout << "Waiting for chainingggg..." << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    
    std::cout << " Test 3 - Done " << std::endl;
}

int main()
{

    TestSingleTask();
    TestTwoTasksPriorityOrder();
    TestTaskAddingTask();
   
    return 0;
}
