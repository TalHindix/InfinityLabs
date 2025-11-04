/*****************************************************************************
* Exercise:    Simple String Exe
* Date:        20/10/2025
* Developer:   Tal Hindi
* Reviewer:    Maria
* Status:      In Progress
*****************************************************************************/
#include <cstring> // strlen
#include <iostream> // cout, cin

#include "string.hpp" // String

using namespace ilrd;

using std::cout;
using std::endl;
using std::cin;


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
    cout << "Enter name: ";
    cin >> name;
    cout << "You entered: " << name << endl;

    // Comparison operators
    if (s1 == s2)
    {
        cout << "Equal" << endl;
    }
    else if (s1 < s2)
    {
        cout << s1 << " comes before " << s2 << endl;
    }

    return 0;
}
