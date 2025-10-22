/*****************************************************************************
 * Exercise:    Complex
 * Date:        21/10/2025
 * Developer:   Tal Hindi
 * Reviewer:
 * Status:      In Progress
 *****************************************************************************/

#include <iostream> /* std::cout */

#include "complex.hpp" /* Complex */

using namespace ilrd;

static void TestGetAndSet();
static void TestCompoundOperators();
static void TestBinaryOperators();
static void TestStreamOperators();

int main()
{
    TestGetAndSet();
    TestCompoundOperators();
    TestBinaryOperators();
    TestStreamOperators();
    return 0;
}

static void TestGetAndSet()
{
    ilrd::Complex s1;

    std::cout << "Starting s1 real value: " << s1.GetReal() << std::endl;
    std::cout << "Starting s1 imaginary value: " << s1.GetImaginary()
              << std::endl;

    s1.SetReal(10.0);
    s1.SetImaginary(5.0);

    std::cout << "After set s1 real value: " << s1.GetReal() << std::endl;
    std::cout << "After set s1 imaginary value: " << s1.GetImaginary()
              << std::endl;
}

static void TestCompoundOperators()
{
    std::cout << std::endl
              << "-------- Compound Operators Test --------" << std::endl;
    ilrd::Complex s1(2, 2);
    ilrd::Complex s2(3, 5);

    std::cout << "-------- Operator '+=' --------" << std::endl;
    std::cout << "s1 before: ";
    s1.Print();
    std::cout << "s2:        ";
    s2.Print();
    std::cout << "s1 after:  ";
    s1 += s2;
    s1.Print();

    std::cout << std::endl << "-------- Operator '-=' --------" << std::endl;
    std::cout << "s1 before: ";
    s1.Print();
    std::cout << "s2:        ";
    s2.Print();
    std::cout << "s1 after:  ";
    s1 -= s2;
    s1.Print();

    std::cout << std::endl << "-------- Operator '*=' --------" << std::endl;
    std::cout << "s1 before: ";
    s1.Print();
    std::cout << "s2:        ";
    s2.Print();
    std::cout << "s1 after:  ";
    s1 *= s2;
    s1.Print();

    std::cout << std::endl << "-------- Operator '/=' --------" << std::endl;
    std::cout << "s1 before: ";
    s1.Print();
    std::cout << "s2:        ";
    s2.Print();
    std::cout << "s1 after:  ";
    s1 /= s2;
    s1.Print();
}

static void TestBinaryOperators()
{
    std::cout << std::endl
              << "-------- Binary Operators Test --------" << std::endl;
    ilrd::Complex s1(2, 2);
    ilrd::Complex s2(3, 5);
    ilrd::Complex s3;

    std::cout << "-------- Operator '+' --------" << std::endl;
    std::cout << "s1 before: ";
    s1.Print();
    std::cout << "s2:        ";
    s2.Print();
    std::cout << "s3 before:   ";
    s3.Print();
    std::cout << "s3 after:  ";
    s3 = s1 + s2;
    s3.Print();

    std::cout << "-------- Operator '-' --------" << std::endl;
    std::cout << "s1 before: ";
    s1.Print();
    std::cout << "s2:        ";
    s2.Print();
    std::cout << "s3 before:   ";
    s3.Print();
    std::cout << "s3 after:  ";
    s3 = s1 - s2;
    s3.Print();

    std::cout << "-------- Operator '*' --------" << std::endl;
    std::cout << "s1 before: ";
    s1.Print();
    std::cout << "s2:        ";
    s2.Print();
    std::cout << "s3 before:   ";
    s3.Print();
    std::cout << "s3 after:  ";
    s3 = s1 * s2;
    s3.Print();

    std::cout << "-------- Operator '/' --------" << std::endl;
    std::cout << "s1 before: ";
    s1.Print();
    std::cout << "s2:        ";
    s2.Print();
    std::cout << "s3 before:   ";
    s3.Print();
    std::cout << "s3 after:  ";
    s3 = s1 / s2;
    s3.Print();
}

static void TestStreamOperators()
{
    std::cout << std::endl
              << "-------- Stream Operators Test --------" << std::endl;
   
   ilrd::Complex s1;
   ilrd::Complex s2(2,5);

   std::cout << s1 << std::endl;


}