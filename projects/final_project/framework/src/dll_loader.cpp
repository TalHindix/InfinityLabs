/*****************************************************************************
 * Exercise:    DllLoader
 * Date:        11/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Avi Tobar
 * Status:      Approved
 *****************************************************************************/

#include <dlfcn.h> // dlopen

#include "logger.hpp"
#include "dll_loader.hpp"

namespace ilrd
{


DllLoader::DllLoader()
{
  DLLLOADER_LOG(Logger::DEBUGING,"CTOR Started...");
}

DllLoader::~DllLoader() noexcept
{
  DLLLOADER_LOG(Logger::DEBUGING,"DTOR Started...");
}

void* DllLoader::Load(const std::string& plugin_name)
{
    DLLLOADER_LOG(Logger::DEBUGING,"Load("+plugin_name+")");
    void* handle = dlopen(plugin_name.c_str(), RTLD_NOW | RTLD_GLOBAL);
    
    if (nullptr == handle)
    {
        return nullptr;
    }
      
    return handle;
}

void DllLoader::LoadCallback(const std::string& plugin_name)
{
  DLLLOADER_LOG(Logger::DEBUGING,"LoadCallback("+plugin_name+")");
  Load(plugin_name);
}

} // namespace ilrd