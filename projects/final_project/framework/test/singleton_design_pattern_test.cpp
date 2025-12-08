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

    Derived* d1 = Derived::GetInstance();
    std::cout << "Instance 1 acquired." << std::endl;

    Derived* d2 = Derived::GetInstance();
    std::cout << "Instance 2 acquired." << std::endl;

    d1->DoWork();
    d2->DoWork();

    return 0;
}