/*****************************************************************************
 * Exercise:    DllLoader
 * Date:        11/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Avi Tobar
 * Status:      Approved
 *****************************************************************************/

#include <dlfcn.h> // dlopen

#include "dll_loader.hpp"

namespace ilrd
{


DllLoader::DllLoader()
{
}

DllLoader::~DllLoader() noexcept
{
}

void* DllLoader::Load(const std::string& plugin_name)
{
    void* handle = dlopen(plugin_name.c_str(), RTLD_NOW | RTLD_GLOBAL);
    
    if (nullptr == handle)
    {
        return nullptr;
    }
      
    return handle;
}

void DllLoader::LoadCallback(const std::string& plugin_name)
{
  Load(plugin_name);
}

} // namespace ilrd