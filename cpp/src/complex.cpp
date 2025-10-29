/*****************************************************************************
 * Exercise:    Complex
 * Date:        21/10/2025
 * Developer:   Tal Hindi
 * Reviewer:
 * Status:      In Progress
 *****************************************************************************/

#include "complex.hpp" // Complex class

namespace ilrd
{

    // Constructor implementation
    Complex::Complex(double real, double imaginary)
        : m_real(real), m_imaginary(imaginary)
    {
        // empty
    }

    void Complex::Print() const
    {
        std::cout << *this << std::endl;
    }

    // by ref
    Complex AddByRef(const Complex& lhs, const Complex& rhs)
    {
        Complex result(lhs);
        result += rhs;
        return result;
    }

    // by val
    Complex AddByValue(Complex lhs,Complex rhs)
    {
        lhs += rhs;
        return lhs;
    }

} // namespace ilrd