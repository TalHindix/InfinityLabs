/*****************************************************************************
 * Exercise:    RC String 
 * Date:        27/10/2025
 * Developer:   Tal Hindi
 * Reviewer:    Guy Argaman
 * Status:      Approved
 *****************************************************************************/

#include "rc_string.hpp"

#include <cstring> // strlen
#include <new>     // new
#include <cstddef> // offstof

using namespace ilrd;

RCString::RCString(const char* str) : m_rc(Init(str))
{
}

RCString::RCString(const RCString& other) : m_rc(other.m_rc)
{
    ++m_rc->m_count;
}

RCString& RCString::operator=(const RCString& other)
{
    if (m_rc == other.m_rc)
    {
        return *this;
    }

    ++other.m_rc->m_count;

    if (m_rc->m_count == 1)
    {
        operator delete(m_rc);
    }
    else
    {
        --m_rc->m_count;
    }

    m_rc = other.m_rc;

    return *this;
}

RCString::~RCString() noexcept
{
    if (m_rc->m_count == 1)
    {
        operator delete(m_rc);
    }
    else
    {
        --m_rc->m_count;
    }
}

char RCString::operator[](size_t idx) const
{
    return m_rc->m_str[idx];
}

char& RCString::operator[](size_t idx)
{
    if (m_rc->m_count > 1)
    {
        --m_rc->m_count;
        m_rc = Init(m_rc->m_str);
    }

    return m_rc->m_str[idx];
}

std::ostream& operator<<(std::ostream& os, const RCString& other)
{
    return os << other.ToCStr();
}

std::istream& operator>>(std::istream& is, RCString& other)
{
    char buff[BUFSIZ];
    std::memset(buff, 0, BUFSIZ);

    is >> buff;
    other = RCString(buff);

    return is;
}

RCString::RCImp* RCString::Init(const char* str)
{
    size_t len = std::strlen(str);
    void* memory = operator new(offsetof(RCImp, m_str) + len + 1);

    RCImp* rc = reinterpret_cast<RCImp*>(memory);
    std::strcpy(rc->m_str, str);
    rc->m_count = 1;

    return rc;
}
