/*****************************************************************************
 * Exercise:    Framework
 * Date:        24/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#include "framework.hpp"
#include "logger.hpp"
#include "thread_pool.hpp"

namespace ilrd
{

Framework::Framework(const std::vector<FdEntry>& entries,
                     std::shared_ptr<Reactor::IListener> listener,
                     const std::vector<CommandCreator>& commands,
                     const std::string& pluginDir)
                    : m_mediator(entries, listener)      
                    , m_dirMonitor(pluginDir)                              
                    , m_pluginCallback(m_dllLoader, &DllLoader::LoadCallback) 
{
    FRAMEWORK_LOG(Logger::DEBUGING, "Ctor - pluginDir: " + pluginDir);

    RegisterCommands(commands);

    m_dirMonitor.Register(&m_pluginCallback);
}


Framework::~Framework()
{
    FRAMEWORK_LOG(Logger::DEBUGING, "Dtor");
    
    m_dirMonitor.Unregister(&m_pluginCallback);
}

void Framework::Run()
{
    FRAMEWORK_LOG(Logger::DEBUGING, "Run()");
    Handleton<ThreadPool>::GetInstance()->Run();
    m_dirMonitor.Run();
    m_mediator.Run();
}

void Framework::Stop()
{
    FRAMEWORK_LOG(Logger::DEBUGING, "Stop()");
    
    m_mediator.Stop();
    Handleton<ThreadPool>::GetInstance()->Pause();
}

void Framework::RegisterCommands(const std::vector<CommandCreator>& commands)
{
    auto factory = Handleton<Factory<ICommand, int>>::GetInstance();
    
    for (const CommandCreator& cmd : commands)
    {
        FRAMEWORK_LOG(Logger::DEBUGING, "Registering command key: " + 
                      std::to_string(cmd.first));
        factory->Add(cmd.first, cmd.second);
    }
}

} // namespace ilrd