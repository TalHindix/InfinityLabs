#include <iostream>
#include <memory>

#include "reactor.hpp"
#include "select_listener.hpp"

using namespace ilrd;

int main()
{
    std::shared_ptr<SelectListener> listener = std::make_shared<SelectListener>();
    Reactor r1(listener);
    r1.Add(fileno(stdin),Reactor::READ, 
    [&r1](int fd, Reactor::Mode Mode)
    {
        std::string s1;
        std::cin >> s1;
        std::cout << "S1 value is " << s1 << std::endl;

        if(s1 == "exit")
        {
            r1.Stop();
        }
    });

    std::cout << "Running..." << std::endl;

    r1.Run();

    std::cout << "Stopped..." << std::endl;

    return 0;
}