/*****************************************************************************
* Exercise:    Simple String Exe
* Date:        20/10/2025
* Developer:   Tal Hindi
* Reviewer:    
* Status:      In Progress
*****************************************************************************/

#include "../inc/string.hpp"

#include <cstring>

namespace ilrd
{
String::String(const char* str) : m_str(0)
{
    m_str = new char[::strlen(str) + 1];
    ::strcpy(m_str, str);
}
String::String(const String& other) : m_str(0)
{
    m_str = new char[::strlen(other.m_str) + 1];
    ::strcpy(m_str, other.m_str);
}
String& String::operator=(const String& other)
{
    if (this != &other)
    {
        char* newStr = new char[::strlen(other.m_str + 1)];
        ::strcpy(newStr, other.m_str);

        delete[] m_str;
        m_str = newStr;
    }
    return *this;
}

String::~String()
{
    delete[] m_str;
}

size_t String::Length() const
{
    return ::strlen(m_str);
}

char* String::Cstr() const
{
    return m_str;
}

bool operator==(const String& lhs, const String& rhs)
{
    return !strcmp(lhs.Cstr(), rhs.Cstr());
}

bool operator>(const String& lhs, const String& rhs)
{
    return (strcmp(lhs.Cstr(), rhs.Cstr()) > 0);
}

bool operator<(const String& lhs, const String& rhs)
{
    return (strcmp(lhs.Cstr(), rhs.Cstr()) < 0);
}

std::ostream& operator<<(std::ostream& os, const String& other)
{
    return os << other.Cstr();
}
std::istream& operator>>(std::istream& is, String& other)
{
    char buffer[256] = {0};

	is >> buffer;
	other = buffer;

	return is;
}

} // namespace ilrd
