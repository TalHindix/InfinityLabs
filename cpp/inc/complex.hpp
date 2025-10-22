/*****************************************************************************
 * Exercise:    Complex
 * Date:        21/10/2025
 * Developer:   Tal Hindi
 * Reviewer:
 * Status:      In Progress
 *****************************************************************************/

#ifndef __ILRD_COMPLEX__
#define __ILRD_COMPLEX__

#include <cmath>    // std::abs
#include <iostream> // std::ostream

namespace ilrd
{

    class Complex
    {
    public:
        Complex(double real = 0,
                double imaginary = 0); // non-explicit on purpose

        inline double GetReal() const
        {
            return m_real;
        }

        inline double GetImaginary() const
        {
            return m_imaginary;
        }

        inline void SetReal(double real)
        {
            m_real = real;
        }

        inline void SetImaginary(double imaginary)
        {
            m_imaginary = imaginary;
        }

        void Print() const;

        inline Complex& operator+=(const Complex& rhs);
        inline Complex& operator-=(const Complex& rhs);
        inline Complex& operator*=(const Complex& rhs);
        inline Complex& operator/=(const Complex& rhs);

    private:
        double m_real;
        double m_imaginary;
    };

    // Compound assignment operators - implementation
    inline Complex& Complex::operator+=(const Complex& rhs)
    {
        m_real += rhs.m_real;
        m_imaginary += rhs.m_imaginary;
        return *this;
    }

    inline Complex& Complex::operator-=(const Complex& rhs)
    {
        m_real -= rhs.m_real;
        m_imaginary -= rhs.m_imaginary;
        return *this;
    }

    inline Complex& Complex::operator*=(const Complex& rhs)
    {
        double newReal = m_real * rhs.m_real - m_imaginary * rhs.m_imaginary;
        double newImaginary =
            m_real * rhs.m_imaginary + m_imaginary * rhs.m_real;

        m_real = newReal;
        m_imaginary = newImaginary;

        return *this;
    }

    inline Complex& Complex::operator/=(const Complex& rhs)
    {
        double denominator =
            rhs.m_real * rhs.m_real + rhs.m_imaginary * rhs.m_imaginary;

        double newReal =
            (m_real * rhs.m_real + m_imaginary * rhs.m_imaginary) / denominator;
        double newImaginary =
            (m_imaginary * rhs.m_real - m_real * rhs.m_imaginary) / denominator;

        m_real = newReal;
        m_imaginary = newImaginary;

        return *this;
    }

    // Binary arithmetic operators
    inline Complex operator+(const Complex& lhs, const Complex& rhs)
    {
        Complex result(lhs);
        result += rhs;
        return result;
    }

    inline Complex operator-(const Complex& lhs, const Complex& rhs)
    {
        Complex result(lhs);
        result -= rhs;
        return result;
    }

    inline Complex operator*(const Complex& lhs, const Complex& rhs)
    {
        Complex result(lhs);
        result *= rhs;
        return result;
    }

    inline Complex operator/(const Complex& lhs, const Complex& rhs)
    {
        Complex result(lhs);
        result /= rhs;
        return result;
    }

    // Comparison operators
    inline bool operator==(const Complex& lhs, const Complex& rhs)
    {
        return (lhs.GetReal() == rhs.GetReal()) &&
               (lhs.GetImaginary() == rhs.GetImaginary());
    }

    inline bool operator!=(const Complex& lhs, const Complex& rhs)
    {
        return !(lhs == rhs);
    }

    // Stream operators
    inline std::ostream& operator<<(std::ostream& os, const Complex& other)
    {
        os << other.GetReal();

        if (other.GetImaginary() >= 0)
        {
            os << " + " << other.GetImaginary() << "i";
        }
        else
        {
            os << " - " << (-other.GetImaginary()) << "i";
        }

        return os;
    }

    inline std::istream& operator>>(std::istream& is, Complex& other)
    {
        double real = 0;
        double imaginary = 0;
        is >> real >> imaginary;

        other.SetReal(real);
        other.SetImaginary(imaginary);

        return is;
    }

} // namespace ilrd
#endif //__ILRD_COMPLEX__