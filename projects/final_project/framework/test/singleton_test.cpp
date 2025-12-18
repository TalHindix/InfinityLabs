#include <iostream>
#include <dlfcn.h>

#include "singleton.hpp"
#include "logger.hpp"

using namespace ilrd;

int main()
{
    std::cout << "=== Main Process ===" << std::endl;
    
    Logger* logger = Singleton<Logger>::GetInstance();
    logger->Log("Hello from main");
    logger->Log("Second message");
    
    std::cout << "\n=== Loading Plugin ===" << std::endl;
    
    void* handle = dlopen("libsingleton_plugin.so", RTLD_NOW);
    if (!handle)
    {
        std::cerr << "dlopen failed: " << dlerror() << std::endl;
        return 1;
    }

    typedef void (*PluginFunc)();
    PluginFunc pluginRun = (PluginFunc)dlsym(handle, "PluginRun");
    
    if (pluginRun)
    {
        pluginRun();
    }


    dlclose(handle);
    return 0;
}