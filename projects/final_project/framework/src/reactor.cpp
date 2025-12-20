/*****************************************************************************
 * Exercise:    Reactor
 * Date:        15/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Yuval Hochman
 * Status:      
 *****************************************************************************/

#include "reactor.hpp"

namespace ilrd
{

Reactor::Reactor(std::shared_ptr<IListener> listener)
    : m_listener(listener)
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

    while (m_is_running && !m_callbacks.empty())
    {
        std::vector<FdPair> fds = GetMonitoredFds();
        if (fds.empty()) break;

        m_listener->Listen(fds);

        for (std::size_t i = 0; i < fds.size() && m_is_running; ++i)
        {
            InvokeCallback(fds[i]);
        }
    }
}

void Reactor::InvokeCallback(const FdPair& fdPair)
{
    CallbackMap::iterator it = m_callbacks.find(fdPair);
    
    if (it != m_callbacks.end())
    {
        it->second(fdPair.first, fdPair.second);
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