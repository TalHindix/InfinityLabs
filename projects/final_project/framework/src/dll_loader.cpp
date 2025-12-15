/*****************************************************************************
 * Exercise:    DllLoader
 * Date:        11/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#include <dlfcn.h> // dlopen

#include "dll_loader.hpp"

namespace ilrd
{

static const char* PLUGIN_INIT_FUNC = "PluginInit";

DllLoader::DllLoader()
{
}

DllLoader::~DllLoader() noexcept
{
}

void* DllLoader::Load(const std::string& plugin_name)
{
    void* handle = dlopen(plugin_name.c_str(), RTLD_NOW | RTLD_GLOBAL);
    
    if (!handle)
    {
        return nullptr;
    }
    
    void* initFunc = dlsym(handle, PLUGIN_INIT_FUNC);
    
    if (nullptr != initFunc)
    {
        typedef void (*InitFunc)();
        InitFunc init = reinterpret_cast<InitFunc>(initFunc);
        init();
    }
    
    return handle;
}

void DllLoader::LoadCallback(const std::string& plugin_name)
{
    Load(plugin_name);
}

} // namespace ilrd