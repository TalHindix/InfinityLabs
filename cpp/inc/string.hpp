/*****************************************************************************
* Exercise:    Simple String Exe
* Date:        20/10/2025
* Developer:   Tal Hindi
* Reviewer:    
* Status:      In Progress
*****************************************************************************/

#ifndef __ILRD_STRING__
#define __ILRD_STRING__

#include <cstddef>
#include <ostream>
#include <istream>

namespace ilrd
{
    class String
    {
    public:
        // Non-explicit to allow implicit conversion from const char*
        String(const char* str = "");

        // Copy constructor: Creates deep copy of other String
        String(const String& other);

        // Assignment operator: Assigns one String to another with deep copy
        String& operator=(const String& other);

        // Destructor: Release Allocated memory .
         ~String();

        // Returns the length of the string .
        size_t Length() const;

        // Return pointer to the m_str;
        char* Cstr() const;
        
    private:
        char* m_str;
        // helper functions
        char* AllocateAndCopy(const char* str);
    };// String class
    
    // Comparison functions 
    bool operator==(const String& lhs, const String& rhs);
    bool operator>(const String& lhs, const String& rhs);
    bool operator<(const String& lhs, const String& rhs);

    // Operator functions 
    std::ostream& operator<<(std::ostream& os, const String& other); //advanced
    std::istream& operator>>(std::istream& is, String& other); //advanced
}// ilrd

#endif //__ILRD_STRING__