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
    
class SelectListener : public Reactor::IListener
{
public:
    std::vector<Reactor::FdPair> Listen(const std::vector<Reactor::FdPair>& set) override;
};

} // ilrd
