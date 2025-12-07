/*****************************************************************************
 Exercise:    Singleton Design Pattern - test
 Date:        06/12/2025
 Developer:   Tal Hindi
 Reviewer:    
 Status:      
 *****************************************************************************/

#include "singleton_design_pattern.hpp"
#include <iostream>

using namespace ilrd;

void Derived::DoWork()
{
    std::cout << "Derived::DoWork called. This object's address is: " << this << std::endl;
}

int main()
{
    std::cout << "--- Testing Singleton Implementation ---" << std::endl;

    auto& instance1 = Derived::GetInstance();
    std::cout << "Instance 1 acquired." << std::endl;

    auto& instance2 = Derived::GetInstance();
    std::cout << "Instance 2 acquired." << std::endl;

    instance1.DoWork();
    instance2.DoWork();

    return 0;
}