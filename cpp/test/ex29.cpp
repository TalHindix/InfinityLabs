/*****************************************************************************
* Exercise:    ex28
* Date:        20/10/2025
* Developer:   Tal Hindi
* Reviewer:     
* Status:      In Progress
*****************************************************************************/

#include <iostream>

class X
{
public:
    explicit X();
    explicit X(int a_);
    explicit X(int a_, int b_);

    operator int() const;
    void Print() const;

private:
    int m_a;
}; 

X::X() : m_a(0) { }
X::X(int a_) : m_a(a_) { }
X::X(int a_, int b_) : m_a(a_ + b_) { }

X::operator int()const 
{ 
    return m_num;
}

void X::Print() 
{ 
    std::cout << "X::Print() " << m_a << std::endl;
}

void Fifi(X x_)
{
    std::cout << "Fifi() " << x_ << std::endl;
    x_.Print();
}


int main()
{
    X x1(7);

    Fifi(x1);     // (1)
    Fifi(X(7));   // (2)
    //Fifi(9);      // (3)
    // Fifi(3, 4); // (4)

    return x1 * 3; //(5)
}