#include <iostream>

struct X
{
    explicit X(); // Initialization function without parameters is named default constructor (often abbreviated as: default ctor).
    explicit X(int a_, int b_ = 8); // Regular (non default) Ctor.
    ~X(); //Deinitialization function is called destructor (Dtor).
    X(const X& other_); //Copy initialization function is called copy costructor or CCtor.


    int m_a;
    const int m_b;
};

struct Y
{
    X m_x; 
    int m_i;

    Y& operator=(const Y& other_)
    {
        m_x.m_a = other_.m_x.m_a;  // Bypass X::operator=
        // m_x.m_b stays unchanged (const)
        m_i = other_.m_i;
        return *this;
    }
};

X::X(): m_a(3), m_b(4) //Implementation of the default Ctor. Why the X::X? The code after the colon is called an initialization list.
{
    // m_a = 3; 
    // m_b = 4;
    std::cout << "this:" << this <<
            " X default Ctor. m_a:" << m_a << 
            " m_b:" << m_b << std::endl;
}

X::X(int a_, int b_): m_a(a_), m_b(b_) //Implementation of the second Ctor. 
{
    std::cout<< "this:" << this <<
        " X int int Ctor. m_a:" << m_a << 
        " m_b:" << m_b << std::endl;
}

X::X(const X& other_): m_a(other_.m_a), m_b(other_.m_b) //Implementation of the copy Ctor.
{
    std::cout<< "this:" << this <<
        " X copy Ctor. m_a:" << m_a << 
        " m_b:" << m_b << std::endl;
}


X::~X()
{
    std::cout << "this:" << this <<
        " X Dtor. m_a:" << m_a << 
        " m_b:" << m_b << std::endl;    
}
int main()
{
    X x1;
    X x2(7);
    X *px = new X(x2);
    X x3(9,10);
    X x4(x1);

    // x1 = x3;

    delete px; px = 0;

    Y y1;
    y1.m_x.m_a=250;
    Y y2(y1);
    Y y3;

    y3=y1;
    return 0;
}