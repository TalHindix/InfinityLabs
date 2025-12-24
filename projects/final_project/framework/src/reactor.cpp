/*****************************************************************************
 * Exercise:    Reactor
 * Date:        15/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Yuval Hochman
 * Status:      
 *****************************************************************************/

#include "reactor.hpp"
#include "logger.hpp"
#include "handleton.hpp"

namespace ilrd
{

Reactor::Reactor(std::shared_ptr<IListener> listener)
    : m_listener(listener)
    , m_is_running(false)
{
    Handleton<Logger>::GetInstance()->Log("Reactor Ctor", Logger::DEBUGING);
}

void Reactor::Add(int fd, Mode mode, CallBack callback)
{
    Handleton<Logger>::GetInstance()->Log("Reactor Add", Logger::DEBUGING);
    FdPair key(fd, mode);
    m_callbacks[key] = callback;
}

void Reactor::Remove(int fd, Mode mode)
{
    Handleton<Logger>::GetInstance()->Log(std::string("Reactor::Remove(fd: ") + std::to_string(fd) + ", mode: " + std::to_string(mode) + ") Started...", Logger::DEBUGING);
    FdPair key(fd, mode);
    m_callbacks.erase(key);
}

void Reactor::Run()
{
    Handleton<Logger>::GetInstance()->Log("Reactor::Run() Started...", Logger::DEBUGING);

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
    Handleton<Logger>::GetInstance()->Log("Reactor::InvokeCallback() Started...", Logger::DEBUGING);

    CallbackMap::iterator it = m_callbacks.find(fdPair);
    
    if (it != m_callbacks.end())
    {
        it->second(fdPair.first, fdPair.second);
    }
}

void Reactor::Stop()
{
    Handleton<Logger>::GetInstance()->Log("Reactor::Stop() Started...", Logger::DEBUGING);
    m_is_running = false;
}

std::vector<Reactor::FdPair> Reactor::GetMonitoredFds() const
{
    Handleton<Logger>::GetInstance()->Log("Reactor::GetMonitoredFds() Started...", Logger::DEBUGING);
    
    std::vector<FdPair> fds;
    fds.reserve(m_callbacks.size());

    for (const CallbackMap::value_type& entry : m_callbacks)
    {
        fds.push_back(entry.first);
    }

    return fds;
}

} // namespace ilrd