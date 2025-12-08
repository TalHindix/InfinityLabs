#include <iostream>
#include "singleton.hpp"
#include "logger.hpp"

using namespace ilrd;

extern "C" void PluginRun()
{
    std::cout << "=== Plugin ===" << std::endl;
    
    Logger* logger = GetSingletonInstance<Logger>();
    logger->Log("Hello from plugin");
    
    std::cout << "Plugin sees count: " << logger->GetCount() << std::endl;
}