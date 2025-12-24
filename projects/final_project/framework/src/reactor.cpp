/*****************************************************************************
 * Exercise:    Reactor
 * Date:        15/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Yuval Hochman
 * Status:      
 *****************************************************************************/

#include "reactor.hpp"
#include "logger.hpp"

namespace ilrd
{

Reactor::Reactor(std::shared_ptr<IListener> listener)
    : m_listener(listener)
    , m_is_running(false)
{
    REACTOR_LOG(Logger::DEBUGING, "Ctor started...");
}

Reactor::~Reactor() noexcept
{
    REACTOR_LOG(Logger::DEBUGING, "Dtor");
}

void Reactor::Add(int fd, Mode mode, CallBack callback)
{
    REACTOR_LOG(Logger::DEBUGING, "Add fd: " + std::to_string(fd));
    FdPair key(fd, mode);
    m_callbacks[key] = callback;
}

void Reactor::Remove(int fd, Mode mode)
{
    REACTOR_LOG(Logger::DEBUGING, "Remove fd: " + std::to_string(fd) + 
                ", mode: " + std::to_string(mode));
    FdPair key(fd, mode);
    m_callbacks.erase(key);
}

void Reactor::Run()
{
    REACTOR_LOG(Logger::DEBUGING, "Run() started");
    
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

    REACTOR_LOG(Logger::DEBUGING, "Run() finished");
}

void Reactor::InvokeCallback(const FdPair& fdPair)
{

    REACTOR_LOG(Logger::DEBUGING,
    std::string("InvokeCallback fd: ") + std::to_string(fdPair.first) +
    " mode: " + ModeToString(fdPair.second));

    CallbackMap::iterator it = m_callbacks.find(fdPair);
    
    if (it != m_callbacks.end())
    {
        it->second(fdPair.first, fdPair.second);
    }
}

void Reactor::Stop()
{
    REACTOR_LOG(Logger::DEBUGING, "Stop()");
    m_is_running = false;
}

std::vector<Reactor::FdPair> Reactor::GetMonitoredFds() const
{
    REACTOR_LOG(Logger::DEBUGING, "GetMonitoredFds()");
    std::vector<FdPair> fds;
    fds.reserve(m_callbacks.size());

    for (const CallbackMap::value_type& entry : m_callbacks)
    {
        fds.push_back(entry.first);
    }

    return fds;
}

const char* Reactor::ModeToString(Reactor::Mode mode)
{
    switch (mode)
    {
        case Reactor::READ:
            return "READ";

        case Reactor::WRITE:
            return "WRITE";

        default:
            return "UNKNOWN_MODE";
    }
}

} // namespace ilrd