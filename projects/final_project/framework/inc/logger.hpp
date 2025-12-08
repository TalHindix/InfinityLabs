/*****************************************************************************
 Exercise:    Singleton - Logger
 Date:        08/12/2025
 Developer:   Tal Hindi
 Reviewer:    
 Status:      
 *****************************************************************************/

#ifndef _ILRD_LOGGER
#define _ILRD_LOGGER

#include <cstddef>
#include <iostream>

namespace ilrd
{
    class Logger
    {
    public:
        Logger() : m_counter(0)
        {
            std::cout << "[Logger Ctor] this: " << this << std::endl;
        }

        void Log(const char* message)
        {
            ++m_counter;
            std::cout << "[Log #" << m_counter << "] " << message << " (this: " << this << ")\n";
        }

        std::size_t GetCount() const
        {
            return m_counter;
        }
            
    private:
      std::size_t m_counter;
    };
}

#endif // _ILRD_LOGGER