
/*****************************************************************************
 * Exercise:    Reactor
 * Date:        11/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#include "select_listener.hpp"

namespace ilrd
{

std::vector<Reactor::FdPair> SelectListener::Listen(const std::vector<Reactor::FdPair>& set)
{
    fd_set read_fds;
    FD_ZERO(&read_fds);

    int max_fd = -1;

    for (const Reactor::FdPair& p : set)
    {
        if (Reactor::READ == p.second)
        {
            FD_SET(p.first, &read_fds);
            max_fd = std::max(max_fd, p.first);
        }
    }

    select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);

    std::vector<Reactor::FdPair> ready;

    for (const Reactor::FdPair& p : set)
    {
        if (Reactor::READ == p.second && FD_ISSET(p.first, &read_fds))
        {
            ready.push_back(p);
        }
    }

    return ready;
}

} // ilrd
