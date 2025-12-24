/*****************************************************************************
 * Exercise:    Framework Test
 * Date:        23/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#include <iostream>
#include <unistd.h>

#include "framework.hpp"
#include "select_listener.hpp"
#include "handleton.hpp"
#include "factory.hpp"
#include "ikeyargs.hpp"
#include "iinput_proxy.hpp"
#include "icommand.hpp"
#include "input_mediator.hpp"

using namespace ilrd;

enum CommandKey
{
    READ_CMD = 0,
    WRITE_CMD = 1
};

class StubKeyArgs : public IKeyArgs
{
public:
    StubKeyArgs(int key) : m_key(key) {}
    
    int 
    GetKey() override 
    { 
        return m_key; 
    }

private:
    int m_key;
};

class StubInputProxy : public IInputProxy
{
public:
    std::shared_ptr<IKeyArgs> GetKeyArgs(int fd, Reactor::Mode mode) override
    {
        (void)mode;
        
        char buffer[256];
        ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
        
        if (bytes > 0)
        {
            buffer[bytes] = '\0';
            std::cout << "[StubInputProxy] Read: " << buffer << std::endl;
        }
        
        return std::make_shared<StubKeyArgs>(READ_CMD);
    }
};

class StubReadCommand : public ICommand
{
public:
async_args Execute(std::shared_ptr<IKeyArgs> task_args) override
    {
        (void)task_args;
        std::cout << "[StubReadCommand] Execute called!" << std::endl;
        
        return {[]() { return true; }, std::chrono::milliseconds(0)};
    }
};

class StubWriteCommand : public ICommand
{
public:
    async_args Execute(std::shared_ptr<IKeyArgs> task_args) override
    {
        (void)task_args;
        std::cout << "[StubWriteCommand] Execute called!" << std::endl;
        
        return {[]() { return true; }, std::chrono::milliseconds(0)};
    }
};

void RegisterCommands()
{
    auto factory = Handleton<Factory<ICommand, int>>::GetInstance();
    
    factory->Add(READ_CMD, []() { 
        return std::make_shared<StubReadCommand>(); 
    });
    
    factory->Add(WRITE_CMD, []() { 
        return std::make_shared<StubWriteCommand>(); 
    });
}

int main()
{
    std::cout << "=== Framework Test ===" << std::endl;
    
    RegisterCommands();
    
    auto listener = std::make_shared<SelectListener>();
    auto proxy = std::make_shared<StubInputProxy>();
    
    std::vector<InputMediator::FdEntry> entries = {
        {STDIN_FILENO, Reactor::READ, proxy}
    };
    
    std::cout << "Type something and press Enter (type 'quit' to exit):" << std::endl;
    
    Framework fw(entries, listener, "./plugins", 2);
    
    fw.Run();
    
    std::cout << "=== Test End ===" << std::endl;
    
    return 0;
}