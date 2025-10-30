// Template functions 

#include <iostream>

template <typename T> 
T Max(T t1, T t2) 
{ 
    return (t1 > t2) ? t1 : t2;  
} 

int main() 
{ 
    int arr[12]; 

    std::cout << Max<int>(3, 5) << " " << Max<double>(3.7, 12.9) << " ";
    std::cout << Max<int*>(arr, arr + 8) << std::endl;
    std::cout << Max('a', 'b') << std::endl;
    std::cout << Max<float>(6.00,10.00) << " ";
    return 0; 
}

// nm output (before remove the calls ):
//                  U _GLOBAL_OFFSET_TABLE_
// 0000000000000000 T main
//                  U __stack_chk_fail
// 0000000000000000 W _Z3MaxIcET_S0_S0_
// 0000000000000000 W _Z3MaxIdET_S0_S0_
// 0000000000000000 W _Z3MaxIiET_S0_S0_
// 0000000000000000 W _Z3MaxIPiET_S1_S1_
//                  U _ZNSolsEd
//                  U _ZNSolsEi
//                  U _ZNSolsEPFRSoS_E
//                  U _ZNSolsEPKv
//                  U _ZSt21ios_base_library_initv
//                  U _ZSt4cout
//                  U _ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
//                  U _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c
//                  U _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc

// nm output (after remove the calls ):
// 0000000000000000 T main
//                  U __stack_chk_fail
//                  U _ZSt21ios_base_library_initv
