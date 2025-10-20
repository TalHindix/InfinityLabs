#include <cstdio>

class X
{
public:
//    explicit X(int a_);
    void Foo();

    int GetA() const
    {
        return m_a;
    }

private:
    unsigned char m_a;
};

// X::X(int a_) : m_a(a_)
// {
// }

void X::Foo()
{
    printf("%d\n", m_a);
}

void Foo(const X& x_)
{
    printf("%d\n", x_.GetA());
}

int main()
{
    X x1;

    x1.Foo();
    printf("%d\n", x1.GetA());
    Foo(x1);

    return 0;
}

// a. error: int X::m_a’ is private within this context
//    error: X x1 no matching function for call to ‘X::X()’
//    error: ‘int X::m_a’ is private within this context