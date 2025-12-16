#include <iostream>

extern "C" void PluginInit() __attribute__((constructor));

extern "C" void PluginInit()
{
    std::cout << "Plugin loaded successfully!" << std::endl;
}
