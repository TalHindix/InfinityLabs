#include <iostream>

void Foo(const double& d)
{
    std::cout << "Foo - address: " << &d << ", value: " << d << std::endl;
}

void Bar(double& d)
{
    std::cout << "Bar - address: " << &d << ", value: " << d << std::endl;
}

int main()
{
    int x = 5;
    std::cout << "x address: " << &x << std::endl << std::endl;
    
    // Test Foo with int
    Foo(x);
    
    // Test Bar with double
    double y = 5.0;
    std::cout << "\ny address: " << &y << std::endl;
    Bar(y);
    
    // Test Bar with reinterpret_cast
    std::cout << std::endl;
    Bar(reinterpret_cast<double&>(x));
    
    return 0;
}