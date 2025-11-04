/*****************************************************************************
 * Exercise:    RC String
 * Date:        27/10/2025
 * Developer:   Tal Hindi
 * Reviewer:    Guy Argaman
 * Status:      Approved
 *****************************************************************************/

#include "rc_string.hpp"

#include <cstddef> // offstof
#include <cstring> // strlen
#include <new>     // new

namespace ilrd
{
RCString::RCString(const char* str): m_rc(CreateMRC(str))
{
}

RCString::RCString(const RCString& other): m_rc(other.IncNShare())
{
}

RCString& RCString::operator=(const RCString& other)
{
    RCImp* rc = other.IncNShare();
    DecOrDel();

    m_rc = rc;

    return *this;
}

RCString::~RCString() noexcept
{
    DecOrDel();
}

char RCString::operator[](size_t idx) const
{
    return m_rc->m_str[idx];
}

char& RCString::operator[](size_t idx)
{
    if (IsShared())
    {
        RCImp* rc = CreateMRC(m_rc->m_str);
        DecOrDel();

        m_rc = rc;
    }

    return m_rc->m_str[idx];
}

std::ostream& operator<<(std::ostream& os, const RCString& other)
{
    return os << other.ToCStr();
}

std::istream& operator>>(std::istream& is, RCString& other)
{
    char buffer[BUFFER_SIZE] = {0};

    is >> buffer;
    other = buffer;

    return is;
}

RCString::RCImp* RCString::IncNShare() const
{
    ++(m_rc->m_count);

    return m_rc;
}

void RCString::DecOrDel()
{
    if (IsShared())
    {
        --m_rc->m_count;
    }
    else
    {
        delete m_rc;
    }

    m_rc = NULL;
}

RCString::RCImp* RCString::CreateMRC(const char* src)
{
    RCImp* rc = static_cast<RCImp*>(operator new(offsetof(RCImp, m_str) +
                                                 strlen(src) + 1));

    rc->m_count = 1;
    strcpy(rc->m_str, src);

    return rc;
}

int RCString::IsShared() const
{
    return m_rc->m_count > 1;
}
} // namespace ilrd
