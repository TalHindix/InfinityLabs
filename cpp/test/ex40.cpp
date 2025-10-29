#include <iostream>


class X
{
public:
    virtual ~X() {}

private:
    int m_a;
};

class Y: public X
{
private:
    int m_b;
};

int main()
{
    X *xp= new Y[5];

    delete[] xp;

    return 0;
}