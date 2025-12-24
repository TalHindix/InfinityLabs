#include <iostream>
#include <memory>
#include "icommand.hpp"
#include "factory.hpp"
#include "handleton.hpp"

class HelloCommand : public ilrd::ICommand 
{
public:
    async_args Execute(std::shared_ptr<ilrd::IKeyArgs>) override 
    {
        std::cout << "!!! Hello from Dynamic Plugin !!!" << std::endl;
        return {[](){ return true; }, std::chrono::milliseconds(0)};
    }
};


__attribute__((constructor))
void RegisterPlugin() 
{
    auto factory = ilrd::Handleton<ilrd::Factory<ilrd::ICommand, int>>::GetInstance();
    factory->Add(100, []() { 
        return std::make_shared<HelloCommand>(); 
    });
}