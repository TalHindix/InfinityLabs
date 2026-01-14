/*****************************************************************************
 * Exercise:    Framework
 * Date:        23/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Avi Tobar
 * Status:      Approved
 *****************************************************************************/

#ifndef __ILRD_FRAMEWORK__
#define __ILRD_FRAMEWORK__

#include <vector>   //std::vector
#include <string>   //std::string
#include <memory>   //std::shared_ptr
#include <cstddef>  

#include "input_mediator.hpp" //InputMediator
#include "dir_monitor.hpp"    //DirMonitor
#include "dll_loader.hpp"     //DllLoader
#include "dispatcher.hpp"     //Dispatcher
#include "thread_pool.hpp"    //ThreadPool
#include "icommand.hpp"       //ICommand
#include "factory.hpp"        //Factory
#include "handleton.hpp"      //Handleton

namespace ilrd
{

class Framework
{
public:
    using FdEntry = std::tuple<int, Reactor::Mode, std::shared_ptr<IInputProxy>>;
    using CommandCreator = std::pair<int, std::function<std::shared_ptr<ICommand>()>>;
    
    Framework(const std::vector<FdEntry>& entries, 
              std::shared_ptr<Reactor::IListener> listener, 
              const std::vector<CommandCreator>& commands,
              const std::string& pluginDir);
                
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
    
    void RegisterCommands(const std::vector<CommandCreator>& commands);
    
}; // class Framework

} // namespace ilrd

#endif // __ILRD_FRAMEWORK__