// Inline Functions (1 missing).

#include <iostream>

using std::cout;

inline void foo() // nm output(before add a calling foo()):
{                 //  0000000000000000 T main
                  //  U , _ZSt21ios_base_library_initv
    cout << "Hello-World";
}

int main()
{
    foo();      // nm output (after add a calling foo()):
                /*  0000000000000000 T main
                    0000000000000000 W _Z3foov
                    U _ZSt21ios_base_library_initv
                    U _ZSt4cout
                    U _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
                */

                /*  nm output (after adding the flag -O )
                    gpd ex15.cpp -c -O
                    0000000000000000 r .LC0
                    0000000000000000 T main
                    U _ZSt21ios_base_library_initv
                    U _ZSt4cout
                    U _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
                */
    return 0;
}


//Purpose: To understand how the compile and the linker behave .
/*
 What does inline means ? 
 nm - different between each stage outputs.
 what does the flag -O do ? 
*/