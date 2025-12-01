/*****************************************************************************
 * Exercise:    BitArray
 * Date:        25/11/2025
 * Developer:   Tal Hindi
 * Reviewer:    Baruchi
 * Status:      Approved
 *****************************************************************************/
 #include <iostream>

 #include "bitarray.hpp"


 int main()
 {
    ilrd::BitArray<10> arr;

    arr.Set(5,true);
    std::cout << "Get(3): " << arr.Get(3) << std::endl;  // 0
    std::cout << "Get(5): " << arr.Get(5) << std::endl;  // 1

    // std::cout << "Before SetBit " << arr.Get(2)  << std::endl;  // 0
    // arr[2] = true;
    // std::cout << "After SetBit 'arr[2] = true '" << arr.Get(2)  << std::endl;  // 0

    arr[5] = true;
    bool b = arr[5];
    std::cout << "arr[5]: " << b << std::endl;  // 1

    return 0;
 }
