/*****************************************************************************
 * Exercise:    ICommand
 * Date:        23/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#ifndef __ILRD_ICOMMAND__
#define __ILRD_ICOMMAND__

#include <chrono> // std::chrono
#include <memory> // std::pair
#include <functional> // std::function

#include "ikeyargs.hpp"

namespace ilrd
{
class ICommand
{
public:
    
    ICommand() = default;
    virtual ~ICommand() = default;
    
    using async_args = std::pair<std::function<bool(void)>, std::chrono::milliseconds>;
    virtual async_args Execute(std::shared_ptr<IKeyArgs> task_args) = 0;
    
}; // class ICommand
} // ilrd

#endif  // __ILRD_ICOMMAND__