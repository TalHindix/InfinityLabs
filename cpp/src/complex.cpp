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

} // namespace ilrd