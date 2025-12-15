#include <iostream>

extern "C" void PluginInit()
{
    std::cout << "Plugin loaded successfully!" << std::endl;
}