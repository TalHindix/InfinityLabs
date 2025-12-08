/*****************************************************************************
 Exercise:    Singleton
 Date:        08/12/2025
 Developer:   Tal Hindi
 Reviewer:    
 Status:      
 *****************************************************************************/

#include "singleton.hpp"
#include "logger.hpp"

namespace ilrd
{

template<typename T>
T* GetSingletonInstance()
{
    return Singleton<T>::GetInstance();
}

template class Singleton<Logger>;
template Logger* GetSingletonInstance<Logger>();

} // namespace ilrd