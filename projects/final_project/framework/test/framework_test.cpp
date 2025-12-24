/*****************************************************************************
 * Exercise:    Framework Test
 * Date:        24/12/2025
 * Developer:   Tal Hindi
 * Reviewer:
 * Status:
 *****************************************************************************/

#include <iostream>     // std::cout
#include <unistd.h>     // STDIN_FILENO, read

#include "framework.hpp"
#include "select_listener.hpp"
#include "handleton.hpp"
#include "thread_pool.hpp"
#include "ikeyargs.hpp"
#include "iinput_proxy.hpp"
#include "icommand.hpp"
#include "logger.hpp"

using namespace ilrd;

enum CommandKey
{
    READ_CMD = 0,
    WRITE_CMD = 1
};

Framework* g_framework = nullptr;

class StubKeyArgs : public IKeyArgs
{
public:
    explicit StubKeyArgs(int key) : m_key(key) {}

    int GetKey() override
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
        char buf[256] = {0};
        if (read(fd, buf, sizeof(buf) - 1) <= 0) {
            return std::make_shared<StubKeyArgs>(mode);
        }

        std::string input(buf);
        if (input.find("quit") == 0 && g_framework) 
        {
            g_framework->Stop();
        }

        if (input.find("100") != std::string::npos)
        {
            return std::make_shared<StubKeyArgs>(100);
        }

        return std::make_shared<StubKeyArgs>(mode);
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

int main()
{
    std::cout << "=== Framework Test ===" << std::endl;
    LOG_DEBUG("================================ Framework Test ================================");
    std::vector<Framework::CommandCreator> commands = 
    {
        {READ_CMD,  []() { return std::make_shared<StubReadCommand>(); }},
        {WRITE_CMD, []() { return std::make_shared<StubWriteCommand>(); }},
    };

    auto listener = std::make_shared<ListenSelect>();
    auto proxy = std::make_shared<StubInputProxy>();

    std::vector<Framework::FdEntry> entries =
    {
        Framework::FdEntry(STDIN_FILENO, Reactor::READ, proxy)
    };

    Framework fw(entries, listener, commands, "./plugins");
    g_framework = &fw;

    std::cout << "Type something and press Enter (type 'quit' to exit):" << std::endl;

    fw.Run();

    std::cout << "=== Test End ===" << std::endl;
    LOG_INFO("================================ Test End ================================");

    return 0;
}
