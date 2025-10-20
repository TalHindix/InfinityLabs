#include <iostream>

void Foo();
void Foo(int i);
void Foo(char c);

int main(void)
{
    int x = 5;
    double z = 10;
    char c = 'b';
    unsigned char b = 'b';

    float y = 3;
    int* p = &x;



    Foo(b);

    return 0;
}

void Foo()
{
    std::cout<< "This is Foo() No Arguments " << std::endl;
}

void Foo(int i)
{
    std::cout<< "This is Foo(int " << i << " That get int as parameter " << std::endl;
}

void Foo(char c)
{
     std::cout<< "This is Foo(char " << c << " That get char as parameter " << std::endl;
}
