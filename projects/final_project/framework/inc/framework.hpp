/*****************************************************************************
 * Exercise:    Framework
 * Date:        23/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#ifndef __ILRD_FRAMEWORK__
#define __ILRD_FRAMEWORK__

#include <vector>
#include <string>
#include <memory>
#include <cstddef>

#include "input_mediator.hpp"
#include "dir_monitor.hpp"
#include "dll_loader.hpp"
#include "dispatcher.hpp"
#include "thread_pool.hpp"
#include "icommand.hpp"
#include "factory.hpp"
#include "handleton.hpp"

namespace ilrd
{

class Framework
{
public:
    Framework(const std::vector<InputMediator::FdEntry>& entries,
              std::shared_ptr<Reactor::IListener> listener,
              const std::string& pluginDir,
              std::size_t numThreads = std::thread::hardware_concurrency());
    
    ~Framework();
    
    void Run();
    void Stop();

    Framework(const Framework&) = delete;
    Framework& operator=(const Framework&) = delete;

private:
    InputMediator m_mediator;
    DirMonitor m_dirMonitor;
    DllLoader m_dllLoader;
    Callback<const std::string&, DllLoader> m_pluginCallback;

}; // class Framework

} // namespace ilrd

#endif // __ILRD_FRAMEWORK__