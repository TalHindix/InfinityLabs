/*****************************************************************************
 * Exercise:    IInputProxy
 * Date:        23/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#ifndef __ILRD_IINPUT_PROXY_
#define __ILRD_IINPUT_PROXY_

#include "reactor.hpp" // Reactor::MODE mode
#include "ikeyargs.hpp"

namespace ilrd
{
class IInputProxy
{
public:

   IInputProxy() = default;
   virtual ~IInputProxy() = default;

   virtual std::shared_ptr<IKeyArgs> GetKeyArgs(int fd, Reactor::Mode mode) = 0;
   
}; // class IInputProxy



class NBDProxy : IInputProxy
{
    
}; // class NBDProxy

} // ilrd

#endif // __ILRD_IINPUT_PROXY_