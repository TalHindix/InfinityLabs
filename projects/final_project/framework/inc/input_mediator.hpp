/*****************************************************************************
 * Class:       InputMediator
 * Date:        23/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#ifndef __ILRD_INPUTMEDIATOR__
#define __ILRD_INPUTMEDIATOR__

#include <tuple>            // std::tuple
#include <vector>           // std::vector
#include <memory>           // std::shared_ptr

#include "thread_pool.hpp"  // ThreadPool
#include "iinput_proxy.hpp" // IInputProxy
#include "reactor.hpp"      // Reactor

namespace ilrd
{

class InputMediator
{
public:

    using Cb = std::shared_ptr<IInputProxy>; 
    using FdEntry = std::tuple<int, Reactor::Mode, Cb>;

    InputMediator(const std::vector<FdEntry>& entries,
                  std::shared_ptr<Reactor::IListener> listener);
    
    ~InputMediator();
    
    void Run(); 
    void Stop();
    
    InputMediator(const InputMediator&) = delete;
    InputMediator& operator=(const InputMediator&) = delete;
    
private:

    class TPTask : public ThreadPool::ITask
    {
    public:
        TPTask(std::shared_ptr<IKeyArgs> args);
        virtual ~TPTask();
        virtual void Execute() override;
       
    private:
        std::shared_ptr<IKeyArgs> m_args;
        
    }; // class TPTask
   
    class ReactorCallback
    {
    public:
        ReactorCallback(Cb inputProxy);
       
        void operator()(int fd, Reactor::Mode mode);
       
    private:
        const Cb m_inputProxy;
        
    }; // class ReactorCallback
    
    Reactor m_reactor;
    
}; // class InputMediator

} // namespace ilrd

#endif  // __ILRD_INPUTMEDIATOR__