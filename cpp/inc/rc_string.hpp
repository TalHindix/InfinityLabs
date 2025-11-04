
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

const size_t BUFFER_SIZE = 256;

class RCString
{
public:
 	RCString(const char* str = ""); //non-explicit on purpose
	RCString(const RCString& other);
	~RCString() noexcept;

	RCString& operator=(const RCString& other);
	char operator[](size_t idx) const;
	char& operator[](size_t idx);

	inline const char* ToCStr() const noexcept;
	inline size_t Length() const noexcept;
private:
	struct RCImp
	{
		size_t m_count;
		char m_str[1];
	} *m_rc;

	RCImp* CreateMRC(const char* src);
	RCImp* IncNShare() const;
	void DecOrDel();
	int IsShared() const;
}; //RCString class

inline bool operator==(const RCString& lhs, const RCString& rhs);
inline bool operator!=(const RCString& lhs, const RCString& rhs);
inline bool operator>(const RCString& lhs, const RCString& rhs);
inline bool operator<(const RCString& lhs, const RCString& rhs);

std::ostream& operator<<(std::ostream& os, const RCString& other);
std::istream& operator>>(std::istream& is, RCString& other);

inline const char* RCString::ToCStr() const noexcept
{
	return m_rc->m_str;
}

inline size_t RCString::Length() const noexcept
{
	return std::strlen(m_rc->m_str);
}

inline bool operator==(const RCString& lhs, const RCString& rhs)
{
	return !std::strcmp(lhs.ToCStr(), rhs.ToCStr());
}

inline bool operator!=(const RCString& lhs, const RCString& rhs)
{
	return !(lhs == rhs);
}

inline bool operator>(const RCString& lhs, const RCString& rhs)
{
	return (0 < std::strcmp(lhs.ToCStr(), rhs.ToCStr()));
}

inline bool operator<(const RCString& lhs, const RCString& rhs)
{
	return (0 > std::strcmp(lhs.ToCStr(), rhs.ToCStr()));
}
} //ilrd

#endif //__ILRD_RCSTRING__
