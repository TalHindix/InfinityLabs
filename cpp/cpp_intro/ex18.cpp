#include <iostream>


int main()
{
    int i= 8;
    int& ip= i;
    ip= 9;
    std::cout<< &ip<<std::endl;
    
    return 0;
}

// #include <iostream>
// #include <string>

// int main()
// {
//     // Reference to double
//     double d = 3.14;
//     double& dRef = d;
//     dRef = 2.71;
//     std::cout << "d = " << d << std::endl;  // Output: 2.71

//     // Reference to char
//     char c = 'A';
//     char& cRef = c;
//     cRef = 'B';
//     std::cout << "c = " << c << std::endl;  // Output: B

//     // Reference to string
//     std::string s = "Hello";
//     std::string& sRef = s;
//     sRef = "World";
//     std::cout << "s = " << s << std::endl;  // Output: World

//     // Reference to pointer
//     int x = 10;
//     int* ptr = &x;
//     int*& ptrRef = ptr;
//     *ptrRef = 20;
//     std::cout << "x = " << x << std::endl;  // Output: 20

//     return 0;
// }

// #include <iostream>

// // Reference as parameter
// void Increment(int& num)
// {
//     ++num;  // Modifies the original variable
// }

// // Reference as return value
// int& GetElement(int* arr, int index)
// {
//     return arr[index];  // Returns alias to array element
// }

// // Struct with reference field - REQUIRES INITIALIZATION
// struct Data
// {
//     int& m_ref;  // Must be initialized in constructor
    
//     Data(int& r) : m_ref(r) {}  // Initialize in initializer list
// };

// int main()
// {
//     // Reference as variable
//     int x = 5;
//     int& xRef = x;
//     xRef = 10;
//     std::cout << "x = " << x << std::endl;  // Output: 10
    
//     // Reference as parameter
//     int y = 7;
//     Increment(y);
//     std::cout << "y = " << y << std::endl;  // Output: 8
    
//     // Reference as return value
//     int arr[3] = {1, 2, 3};
//     GetElement(arr, 1) = 99;
//     std::cout << "arr[1] = " << arr[1] << std::endl;  // Output: 99
    
//     // Reference in struct
//     int val = 42;
//     Data d(val);
//     d.m_ref = 100;
//     std::cout << "val = " << val << std::endl;  // Output: 100
    
//     return 0;
// }