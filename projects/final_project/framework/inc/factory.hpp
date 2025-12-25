/*****************************************************************************
 * Exercise:    Factory Design Pattern
 * Date:        04/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Avi Tobar
 * Status:      Approved
 *****************************************************************************/

#ifndef _ILRD_FACTORY_
#define _ILRD_FACTORY_

#include <functional>     // std::function 
#include <memory>         // std::shared_ptr 
#include <stdexcept>      // std::out_of_range 
#include <unordered_map>  // std::unordered_map
#include <string>         // std::string

#include "logger.hpp"     // FACTORY_LOG

namespace ilrd
{

template<typename BASE, typename KEY, typename... ARGS>     
class Factory
{
public:
    using BASE_PTR = std::shared_ptr<BASE>;
    using CTOR = std::function<BASE_PTR(ARGS...)>;

 
    Factory();
    ~Factory() noexcept;

    void Add(const KEY& key, CTOR ctor) noexcept;

    BASE_PTR Create(const KEY& key, ARGS... args);

    Factory(const Factory& other) = delete;
    Factory& operator=(const Factory& other) = delete; 
    Factory(Factory&& other) = delete;
    Factory& operator=(Factory&& other) = delete;    

private:
    std::unordered_map<KEY, CTOR> m_map;
}; // class Factory


template<typename BASE, typename KEY, typename... ARGS>
Factory<BASE, KEY, ARGS...>::Factory()
{
    FACTORY_LOG(Logger::DEBUGING, "Ctor");
}

template<typename BASE, typename KEY, typename... ARGS>
Factory<BASE, KEY, ARGS...>::~Factory() noexcept
{
    FACTORY_LOG(Logger::INFO, "Dtor");
}

template<typename BASE, typename KEY, typename... ARGS>
void Factory<BASE, KEY, ARGS...>::Add(const KEY& key, CTOR ctor) noexcept
{
    FACTORY_LOG(Logger::DEBUGING, "Add()");
    m_map[key] = ctor;
}

template<typename BASE, typename KEY, typename... ARGS>
typename Factory<BASE, KEY, ARGS...>::BASE_PTR 
Factory<BASE, KEY, ARGS...>::Create(const KEY& key, ARGS... args)
{
    auto iter = m_map.find(key);

    if (iter == m_map.end())
    {
        FACTORY_LOG(Logger::DEBUGING, "Key not found!");
        throw std::out_of_range("Factory::Create - key not found");
    }

    FACTORY_LOG(Logger::DEBUGING, "Create()");

    return iter->second(args...);
}

} // namespace ilrd


#endif // _ILRD_FACTORY_