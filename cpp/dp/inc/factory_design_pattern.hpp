/*****************************************************************************
 * Exercise:    Factory Design Pattern
 * Date:        04/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Avi Tobar
 * Status:      
 *****************************************************************************/

#ifndef _ILRD_FACTORY_
#define _ILRD_FACTORY_

#include <functional>     // std::function 
#include <memory>         // std::shared_ptr 
#include <stdexcept>      // std::out_of_range 
#include <unordered_map>  // std::unordered_map

namespace ilrd
{
/*****************************************************************************
 * Factory - A creational pattern template that constructs objects from a 
 *           specific hierarchy according to their corresponding key values.
 * 
 * Template Parameters:
 *   BASE - The base class type of the hierarchy to be created
 *   KEY  - The key type used to identify which derived type to create
 *   ARGS - Variadic template parameters for constructor arguments
 * 
 *****************************************************************************/
template<typename BASE, typename KEY, typename... ARGS>     
class Factory
{
public:
    using BASE_PTR = std::shared_ptr<BASE>;
    using CTOR = std::function<BASE_PTR(ARGS...)>;

 
    Factory() = default;
    ~Factory() noexcept = default;

    /**************************************************************************
     * Add - Registers a creator function for a specific key
     * 
     * Parameters:
     *   key  - The key to associate with this creator
     *   ctor - Function that creates and returns a shared_ptr to BASE
     * 
     * Note: If key already exists, the creator will be overwritten
     * Complexity: O(1) average, O(n) worst case
     *************************************************************************/
    void Add(const KEY& key, CTOR ctor) noexcept;

    /**************************************************************************
     * Create - Creates an object of the type associated with the given key
     * 
     * Parameters:
     *   key  - The key identifying which type to create
     *   args - Arguments to forward to the creator function
     * 
     * Returns: shared_ptr to the newly created object
     * Throws: std::out_of_range if key is not found
     * Complexity: O(1) average, O(n) worst case
     *************************************************************************/
    BASE_PTR Create(const KEY& key, ARGS... args);

    Factory(const Factory& other) = delete;
    Factory& operator=(const Factory& other) = delete; 
    Factory(Factory&& other) = delete;
    Factory& operator=(Factory&& other) = delete;    

private:
    std::unordered_map<KEY, CTOR> m_map;
}; // class Factory


template<typename BASE, typename KEY, typename... ARGS>
void Factory<BASE, KEY, ARGS...>::Add(const KEY& key, CTOR ctor)
{
    m_map[key] = ctor;
}

template<typename BASE, typename KEY, typename... ARGS>
typename Factory<BASE, KEY, ARGS...>::BASE_PTR 
Factory<BASE, KEY, ARGS...>::Create(const KEY& key, ARGS... args)
{
    auto iter = m_map.find(key);

    if (iter == m_map.end())
    {
        throw std::out_of_range("Factory::Create - key not found");
    }

    return iter->second(args...);
}

} // namespace ilrd

#endif // _ILRD_FACTORY_