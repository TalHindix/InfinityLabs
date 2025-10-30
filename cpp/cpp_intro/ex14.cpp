#include <iostream>
#include <cstdio>

// const int i = 3; // r , _ZL1i
// const double blabla = 5.00; // r , _ZL6blabla

// float x = 4; // D , No Name Mangling just x symbol.

int i = 3;
int main()
{
    int *ip = (int*)&i;
    *ip = 5;
    int arr[i]; // Work , Just unused warning.

    printf("%d %d\n", i, *ip);
    return 0;
}

//e. ANSWER: We can stop using Preproccessors macros(#define) or magic numbers,
//           to define array sizes and constants. 