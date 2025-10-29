
/*****************************************************************************
 * Exercise:    RC String
 * Date:        27/10/2025
 * Developer:   Tal Hindi
 * Reviewer:    Guy Argaman
 * Status:      Approved
 *****************************************************************************/

#ifndef __ILRD_RCSTRING__
#define __ILRD_RCSTRING__

#if __cplusplus < 201104
#define noexcept throw()
#endif

#include <cstdio>   // BUFSIZ
#include <cstring>  //strlen
#include <iostream> //std::istream

namespace ilrd
{
class RCString
{
public:
    RCString(const char* str = ""); // non-explicit on purpose
    RCString(const RCString& other);
    RCString& operator=(const RCString& other);
    ~RCString() noexcept;
    inline char* ToCStr() const noexcept;
    inline size_t Length() const noexcept;
    char operator[](size_t idx) const;
    char& operator[](size_t idx);

private:
    struct RCImp
    {
        size_t m_count;
        char m_str[1];
    }* m_rc;

    static RCImp* Init(const char* str);
}; // RCString class

inline bool operator==(const RCString& lhs, const RCString& rhs);
inline bool operator!=(const RCString& lhs, const RCString& rhs);
inline bool operator>(const RCString& lhs, const RCString& rhs);
inline bool operator<(const RCString& lhs, const RCString& rhs);

std::ostream& operator<<(std::ostream& os, const RCString& other);
std::istream& operator>>(std::istream& is, RCString& other);

inline char* RCString::ToCStr() const noexcept
{
    return m_rc->m_str;
}

inline size_t RCString::Length() const noexcept
{
    return strlen(m_rc->m_str);
}

inline bool operator==(const RCString& lhs, const RCString& rhs)
{
    return (0 == strcmp(lhs.ToCStr(), rhs.ToCStr()));
}

inline bool operator!=(const RCString& lhs, const RCString& rhs)
{
    return !(lhs == rhs);
}

inline bool operator>(const RCString& lhs, const RCString& rhs)
{
    return (strcmp(lhs.ToCStr(), rhs.ToCStr()) > 0);
}

inline bool operator<(const RCString& lhs, const RCString& rhs)
{
    return (strcmp(lhs.ToCStr(), rhs.ToCStr()) < 0);
}

} // namespace ilrd

#endif //__ILRD_RCSTRING__