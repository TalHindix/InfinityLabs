/*****************************************************************************
 * Exercise:    Simple String Exe
 * Date:        20/10/2025
 * Developer:   Tal Hindi
 * Reviewer:    Maria
 * Status:      In Progress
 *****************************************************************************/

#ifndef __ILRD_STRING__
#define __ILRD_STRING__

#include <cstddef> // size_t
#include <istream> // istream
#include <ostream> // ostream

namespace ilrd
{
class String
{
public:
    String(const char* str = "");

    String(const String& other);

    String& operator=(const String& other);

    ~String();

    size_t Length() const;

    char* Cstr() const;

private:
    char* m_str;
    char* AllocateAndCopy(const char* str);
}; // String class

// Comparison functions
bool operator==(const String& lhs, const String& rhs);
bool operator>(const String& lhs, const String& rhs);
bool operator<(const String& lhs, const String& rhs);

// Operator functions
std::ostream& operator<<(std::ostream& os, const String& other); // advanced
std::istream& operator>>(std::istream& is, String& other);       // advanced
} // namespace ilrd

#endif //__ILRD_STRING__