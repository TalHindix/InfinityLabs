/*****************************************************************************
 * Exercise:    IKeyargs
 * Date:        23/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#ifndef __ILRD_IKEYARGS__
#define __ILRD_IKEYARGS__

#include "reactor.hpp"

namespace ilrd
{

class IKeyArgs
{
public:
    virtual int GetKey() = 0;
}; // class IKeyArgs

} // ilrd

#endif  // __ILRD_IKEYARGS__

