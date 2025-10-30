#include <iostream>

class X
{
public:
    virtual ~X()
    {
    }
    virtual void Print() const
    {
        std::cout << "X::Print() - m_a = " << m_a << "\n";
    }
private:
    int m_a;
};

class Y: public X
{
    void Print() const
    {
        std::cout << "Y::Print() - m_b = " << m_b << "\n";
    }
private:
    int m_b;
};

void Foo(const X& x);

int main()
{
    std::cout << "Creating Y object:\n";
    Y y1;
    std::cout << "Address of y1: " << &y1 << "\n";
    std::cout << "sizeof(y1): " << sizeof(y1) << "\n";
    
    Foo(y1);
    
    std::cout << "\nBack in main, y1 still alive\n";
    
    return 0;
}

void Foo(const X& x)
{
    std::cout << "\nInside Foo:\n";
    std::cout << "Address of x: " << &x << "\n";
    std::cout << "sizeof(x) via reference: " << sizeof(x) << "\n";
    std::cout << "Calling x.Print():\n";
    x.Print();
}