#include <iostream>

using namespace std;
struct X
{
    X(int a_, int b_);

    void Inc();
    void Dec();

    int m_a;
    int m_b;
};

X::X(int a_, int b_): m_a(a_), m_b(b_)
{}

void X::Inc()
{
    cout << "this:" << this <<endl;
    ++m_a;
    ++m_b;
}

void X::Dec()
{
    cout << "this:" << this <<endl;
    --m_a;
    --m_b;
}

void Inc()
{}

int main()
{
    X x1(7,-55);
    X x2(x1);

    Inc();
    x1.Inc();
    x1.Inc();
    x2.Dec();

    return 0;
}

// $1 = {void (X * const)} 0x555555555172 <X::Inc()>
/* nm output: 
    0000000000000092 T main
                 U __stack_chk_fail
0000000000000087 T _Z3Incv  ----- Global func 
0000000000000058 T _ZN1X3DecEv ----- Member func 
0000000000000028 T _ZN1X3IncEv ----- Member func 
0000000000000000 T _ZN1XC1Eii
0000000000000000 T _ZN1XC2Eii
                 U _ZSt21ios_base_library_initv
*/