/*****************************************************************************
 Exercise:    Singleton
 Date:        08/12/2025
 Developer:   Tal Hindi
 Reviewer:    
 Status:      
 *****************************************************************************/

#ifndef _ILRD_SINGLETON
#define _ILRD_SINGLETON

#include <atomic>   // std::atomic
#include <mutex>    // std::mutex
#include <cstdlib>  // std::atexit

namespace ilrd
{

template<typename T>   
class Singleton
{
public:
    static T* GetInstance()
    {
        T* tmp = s_instance.load(std::memory_order_acquire);
        
        if (nullptr == tmp)
        {
            // Check if already destroyed
            if (s_isDestroyed.load(std::memory_order_acquire))
            {
                std::abort();
            }
            
            std::lock_guard<std::mutex> lock(s_mutex);
            tmp = s_instance.load(std::memory_order_relaxed);
            
            if (nullptr == tmp)
            {
                tmp = new T();
                s_instance.store(tmp, std::memory_order_release);
                std::atexit(Destroy);
            }
        }
        
        return tmp;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

protected:
    Singleton() = default;
    ~Singleton() = default;

private:
    static void Destroy()
    {
        T* tmp = s_instance.load(std::memory_order_acquire);
        s_isDestroyed.store(true, std::memory_order_release);
        s_instance.store(nullptr, std::memory_order_release);
        delete tmp;
    }

    static std::atomic<T*> s_instance;
    static std::mutex s_mutex;
    static std::atomic<bool> s_isDestroyed;
};

template<typename T>
std::atomic<T*> Singleton<T>::s_instance{nullptr};

template<typename T>
std::mutex Singleton<T>::s_mutex;

template<typename T>
std::atomic<bool> Singleton<T>::s_isDestroyed{false};

template<typename T>
T* GetSingletonInstance();

} // namespace ilrd

#endif // _ILRD_SINGLETON