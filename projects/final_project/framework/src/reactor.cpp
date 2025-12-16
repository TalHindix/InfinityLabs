/*****************************************************************************
 * Exercise:    Reactor
 * Date:        15/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#include "reactor.hpp"

namespace ilrd
{

Reactor::Reactor(std::shared_ptr<IListener> listener)
    : m_callbacks()
    , m_listener(listener)
    , m_is_running(false)
{
}

void Reactor::Add(int fd, Mode mode, CallBack callback)
{
    FdPair key(fd, mode);
    m_callbacks[key] = callback;
}

void Reactor::Remove(int fd, Mode mode)
{
    FdPair key(fd, mode);
    m_callbacks.erase(key);
}

void Reactor::Run()
{
    m_is_running = true;

    while (m_is_running)
    {
        std::vector<FdPair> fds = GetMonitoredFds();
        if (fds.empty()) break;

        std::vector<FdPair> ready = m_listener->Listen(fds);

        for (std::size_t i = 0; i < ready.size() && m_is_running; ++i)
        {
            InvokeCallback(ready[i]);
        }
    }
}

void Reactor::InvokeCallback(const FdPair& fdPair)
{
    CallbackMap::iterator it = m_callbacks.find(fdPair);
    
    if (it != m_callbacks.end())
    {
        CallBack call_back = it->second; 
        call_back(fdPair.first, fdPair.second);
    }
}

void Reactor::Stop()
{
    m_is_running = false;
}

std::vector<Reactor::FdPair> Reactor::GetMonitoredFds() const
{
    std::vector<FdPair> fds;
    fds.reserve(m_callbacks.size());

    for (const CallbackMap::value_type& entry : m_callbacks)
    {
        fds.push_back(entry.first);
    }

    return fds;
}



} // namespace ilrd