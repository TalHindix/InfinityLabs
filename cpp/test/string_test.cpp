/*****************************************************************************
* Exercise:    Simple String Exe
* Date:        20/10/2025
* Developer:   Tal Hindi
* Reviewer:    
* Status:      In Progress
*****************************************************************************/

#include "../inc/string.hpp"

#include <iostream>

using std::cout;
using std::endl;

using namespace ilrd;

void Foo(String s)
{
    cout << "Foo Function Output: " << s.Cstr() << endl;
}

String Bla()
{
    return "efg";
}

int main()
{
    // Basic functionality test
    ilrd::String s1("hello");
    ilrd::String s2(s1);
    s1 = s2;
    s1.Length();
    s1.Cstr();
    String tmp = Bla();

    Foo("Tal Hindi Is The King ");
    Foo(tmp);

    ilrd::String name;
    std::cout << "Enter name: ";
    std::cin >> name;
    std::cout << "You entered: " << name << std::endl;
    
    // Comparison operators
    if (s1 == s2)
    {
        std::cout << "Equal" << std::endl;
    }
    else if (s1 < s2)
    {
        std::cout << s1 << " comes before " << s2 << std::endl;
    }

    return 0;
}