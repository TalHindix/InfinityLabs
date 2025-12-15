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


} // namespace ilrd