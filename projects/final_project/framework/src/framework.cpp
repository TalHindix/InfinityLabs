/*****************************************************************************
 * Exercise:    Framework
 * Date:        23/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#include "framework.hpp"
#include "logger.hpp"

namespace ilrd
{

Framework::Framework(const std::vector<InputMediator::FdEntry>& entries,
                     std::shared_ptr<Reactor::IListener> listener,
                     const std::string& pluginDir,
                     std::size_t numThreads)
    : m_mediator(entries, listener)
    , m_dirMonitor(pluginDir)
    , m_pluginCallback(m_dllLoader, &DllLoader::LoadCallback)
{
    Handleton<Logger>::GetInstance()->Log("Framework Ctor", Logger::DEBUGING);

    Handleton<ThreadPool>::GetInstance()->SetNumOfThreads(numThreads);

    m_dirMonitor.Register(&m_pluginCallback);
    m_dirMonitor.Run();
}

Framework::~Framework()
{
    Handleton<Logger>::GetInstance()->Log("Framework Dtor", Logger::DEBUGING);
    
    m_dirMonitor.Unregister(&m_pluginCallback);
}

void Framework::Run()
{
    Handleton<Logger>::GetInstance()->Log("Framework Run", Logger::DEBUGING);

    Handleton<ThreadPool>::GetInstance()->Run();
    m_mediator.Run();
}

void Framework::Stop()
{
    Handleton<Logger>::GetInstance()->Log("Framework Stop", Logger::DEBUGING);

    Handleton<ThreadPool>::GetInstance()->Stop();
}

} // namespace ilrd