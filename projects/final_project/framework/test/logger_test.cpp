/*****************************************************************************
 * Exercise:    Singleton - Logger
 * Date:        08/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#include <iostream>

#include "singleton.hpp"
#include "logger.hpp"

using namespace ilrd;

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RED   "\033[1;31m"

static void PrintResult(bool passed, const std::string& testName)
{
    if (passed)
    {
        std::cout << COLOR_GREEN << "[PASS] " << COLOR_RESET << testName << std::endl;
    }
    else
    {
        std::cout << COLOR_RED << "[FAIL] " << COLOR_RESET << testName << std::endl;
    }
}

static int CountLinesInFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    int count = 0;
    std::string line;
    
    while (std::getline(file, line))
    {
        ++count;
    }
    
    return count;
}

static void TestSingleton()
{
   Logger* logger1 = Singleton<Logger>::GetInstance();
   Logger* logger2 = Singleton<Logger>::GetInstance();

   PrintResult(logger1 == logger2, "Same instance ret");
   PrintResult(logger1 != nullptr, "Instance is not null");
}

static void TestBasicLogging()
{
    std::cout << "\n=== Test: Basic Logging ===" << std::endl;

    Logger* logger = Singleton<Logger>::GetInstance();
    logger->SetLevel(Logger::INFO);

    int linesBefore = CountLinesInFile("./log_file");

    logger->Log("Error message", Logger::ERROR, __FILE__, __LINE__);
    logger->Log("Warning message", Logger::WARNING, __FILE__, __LINE__);
    logger->Log("Debuging message", Logger::DEBUGING, __FILE__, __LINE__);
    logger->Log("Info message", Logger::INFO, __FILE__, __LINE__);

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    int linesAfter = CountLinesInFile("./log_file");
    int newLines = linesAfter - linesBefore;

    PrintResult(newLines == 4, "All log levels written (" + 
                std::to_string(newLines) + "/4)");

}

static void TestLogLevelFiltering()
{
    std::cout << "\n=== Test: Log Level Filtering ===" << std::endl;
    
    Logger* logger = Singleton<Logger>::GetInstance();
    
    int linesBefore = CountLinesInFile("./log_file");
    
    logger->SetLevel(Logger::WARNING);
    
    logger->Log("Should appear - ERROR", Logger::ERROR);
    logger->Log("Should appear - WARNING", Logger::WARNING);
    logger->Log("Should NOT appear - DEBUGING", Logger::DEBUGING);
    logger->Log("Should NOT appear - INFO", Logger::INFO);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    
    int linesAfter = CountLinesInFile("./log_file");
    int newLines = linesAfter - linesBefore;
    
    logger->SetLevel(Logger::INFO);
    
    PrintResult(newLines == 2, "Only ERROR and WARNING logged (" + 
                std::to_string(newLines) + "/2)");
}

static void TestMultiThreadedLogging()
{
    std::cout << "\n=== Test: Multi-Threaded Logging ===" << std::endl;
    
    Logger* logger = Singleton<Logger>::GetInstance();
    logger->SetLevel(Logger::INFO);
    
    int linesBefore = CountLinesInFile("./log_file");
    
    const int NUM_THREADS = 5;
    const int LOGS_PER_THREAD = 20;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back([logger, i]()
        {
            for (int j = 0; j < LOGS_PER_THREAD; ++j)
            {
                logger->Log("Thread " + std::to_string(i) + 
                           " - Message " + std::to_string(j), 
                           Logger::INFO);
            }
        });
    }
    
    for (auto& thread : threads)
    {
        thread.join();
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    int linesAfter = CountLinesInFile("./log_file");
    int newLines = linesAfter - linesBefore;
    int expectedCount = NUM_THREADS * LOGS_PER_THREAD;
    
    PrintResult(newLines >= expectedCount, 
        "Multi-threaded logging (" + std::to_string(newLines) + 
        "/" + std::to_string(expectedCount) + " lines)");
}

int main()
{
   TestSingleton();
   TestBasicLogging();
   TestLogLevelFiltering();
   TestMultiThreadedLogging();
   return 0;
}