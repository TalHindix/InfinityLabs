/*****************************************************************************
 * Exercise:    InputMediator
 * Date:        23/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#include "input_mediator.hpp"
#include "logger.hpp"
#include "handleton.hpp"
#include "icommand.hpp"
#include "factory.hpp"
#include "async_injection.hpp"

namespace ilrd
{

InputMediator::TPTask::TPTask(std::shared_ptr<IKeyArgs> args) 
    : m_args(args)
{
    MEDIATOR_LOG(Logger::DEBUGING, "TPTask Ctor");
}

InputMediator::TPTask::~TPTask()
{
    MEDIATOR_LOG(Logger::DEBUGING, "TPTask Dtor");
}

void InputMediator::TPTask::Execute()
{
    MEDIATOR_LOG(Logger::DEBUGING, "TPTask Execute - key: " + 
                 std::to_string(m_args->GetKey()));

    auto factory = Handleton<Factory<ICommand, int>>::GetInstance();
    auto command = factory->Create(m_args->GetKey());
    auto return_val = command->Execute(m_args);

    new AsyncInjection(return_val.first, return_val.second);
}

InputMediator::ReactorCallback::ReactorCallback(Cb inputProxy) 
    : m_inputProxy(inputProxy)
{
    MEDIATOR_LOG(Logger::DEBUGING, "ReactorCallback Ctor");
}

void InputMediator::ReactorCallback::operator()(int fd, Reactor::Mode mode)
{
    MEDIATOR_LOG(Logger::DEBUGING, "ReactorCallback invoked - fd: " + 
                 std::to_string(fd));

    std::shared_ptr<IKeyArgs> args = m_inputProxy->GetKeyArgs(fd, mode);
    auto task = std::make_shared<TPTask>(args);
    Handleton<ThreadPool>::GetInstance()->Add(task);
}

InputMediator::InputMediator(const std::vector<FdEntry>& entries,
                             std::shared_ptr<Reactor::IListener> listener)
    : m_reactor(listener)
{
    MEDIATOR_LOG(Logger::DEBUGING, "Ctor - registering " + 
                 std::to_string(entries.size()) + " entries");
    
    for (const FdEntry& entry : entries)
    {
        int fd = std::get<0>(entry);
        Reactor::Mode mode = std::get<1>(entry);
        std::shared_ptr<IInputProxy> proxy = std::get<2>(entry);
        
        ReactorCallback callback(proxy);
        m_reactor.Add(fd, mode, callback);
    }   
}

InputMediator::~InputMediator()
{
    MEDIATOR_LOG(Logger::DEBUGING, "Dtor");
    m_reactor.Stop();
}

void InputMediator::Run()
{
    MEDIATOR_LOG(Logger::DEBUGING, "Run()");
    m_reactor.Run();
}

void InputMediator::Stop()
{
    MEDIATOR_LOG(Logger::DEBUGING, "Stop()");
    m_reactor.Stop();
}

} // namespace ilrd