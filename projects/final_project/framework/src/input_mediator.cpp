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
    Handleton<Logger>::GetInstance()->Log("TPTask Ctor", Logger::DEBUGING);
}

InputMediator::TPTask::~TPTask()
{
    Handleton<Logger>::GetInstance()->Log("TPTask Dtor", Logger::DEBUGING);
}

void InputMediator::TPTask::Execute()
{
    Handleton<Logger>::GetInstance()->Log("TPTask Execute", Logger::DEBUGING);

    auto factory = Handleton<Factory<ICommand, int>>::GetInstance();
    auto command = factory->Create(m_args->GetKey());
    auto return_val = command->Execute(m_args);

    new AsyncInjection(return_val.first, return_val.second);
}

InputMediator::ReactorCallback::ReactorCallback(Cb inputProxy) 
    : m_inputProxy(inputProxy)
{
    Handleton<Logger>::GetInstance()->Log("ReactorCallback Ctor", Logger::DEBUGING);
}

void InputMediator::ReactorCallback::operator()(int fd, Reactor::Mode mode)
{
    Handleton<Logger>::GetInstance()->Log("ReactorCallback operator()", Logger::DEBUGING);

    std::shared_ptr<IKeyArgs> args = m_inputProxy->GetKeyArgs(fd, mode);
    auto task = std::make_shared<TPTask>(args);
    Handleton<ThreadPool>::GetInstance()->Add(task);
}

InputMediator::InputMediator(const std::vector<FdEntry>& entries,
                             std::shared_ptr<Reactor::IListener> listener)
    : m_reactor(listener)
{
    Handleton<Logger>::GetInstance()->Log("InputMediator Ctor", Logger::DEBUGING);
    
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
    m_reactor.Stop();
    Handleton<Logger>::GetInstance()->Log("InputMediator Dtor", Logger::DEBUGING);
}

void InputMediator::Run()
{
    Handleton<Logger>::GetInstance()->Log("InputMediator Run", Logger::DEBUGING);
    m_reactor.Run();
}

} // namespace ilrd