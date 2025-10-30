#include <iostream>
#include <typeinfo>
#include <string>

struct X 
{ 
    explicit X(int); 
    ~X(); 
    void Foo(); 
    void Bar() const; 

    int m_a; 
    int *m_p; 
}; 

X::X(int a_): m_a( a_ ), m_p(new int(a_)) {} 

X::~X() { delete m_p; m_p=0; } 

void X::Foo() { ++m_a; --(*m_p); } 

void X::Bar() const 
{ 
    // (gdb) p this output > $1 = (const X * const) 0x7fffffffd930
    // (gdb) whatis this output > type = const X * const
    //  
    std::cout << m_a << std::endl; 
    std::cout << *m_p << std::endl; 
    std::cout << m_p << std::endl;
    std::cout << "Type of *this (const object): " << typeid(*this).name() << std::endl;
        
    std::cout << "Value of this (Memory Address): " << this << std::endl;
    //m_a = 0; //---1--- // Assigment of member X::m_a in read-only object .
    //m_p = 0; //---2--- // read only object.
    //*m_p = 0; //---3--- // Work 
    //Foo(); //---5--- error: passing ‘const X’ as ‘this’ argument discards qualifiers
} 

void Fifi(const X& x_) 
{ 
    //x_.Foo(); //---4---
    x_.Bar(); 
} 

int main() 
{ 
    X x1(1); 

    x1.Foo(); 
    Fifi(x1); 

    return 0; 
}