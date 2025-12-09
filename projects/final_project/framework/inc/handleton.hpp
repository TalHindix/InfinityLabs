/*****************************************************************************
 Exercise:    Handleton
 Date:        08/12/2025
 Developer:   Tal Hindi
 Reviewer:    
 Status:      
 *****************************************************************************/

#ifndef __ILRD_HANDLETON__
#define __ILRD_HANDLETON__

#include <mutex>    // std::mutex
#include <atomic>   // std::atomic
#include <cstdlib>  // std::atexit

namespace ilrd
{

template<typename T>
class Handleton
{
public:
    static T* GetInstance();

    Handleton(const Handleton&) = delete;
    Handleton& operator=(const Handleton&) = delete;
    Handleton(Handleton&&) = delete;
    Handleton& operator=(Handleton&&) = delete;

protected:
    Handleton() = default;
    ~Handleton() = default;

private:
    static void Destroy();

    static std::atomic<T*> s_instance;
    static std::mutex s_mutex;
    static std::atomic<bool> s_isDestroyed;
}; // class Handleton

template<typename T>
std::atomic<T*> Handleton<T>::s_instance{nullptr};

template<typename T>
std::mutex Handleton<T>::s_mutex;

template<typename T>
std::atomic<bool> Handleton<T>::s_isDestroyed{false};

#ifdef __HANDLETON__

template<typename T>
T* Handleton<T>::GetInstance()
{
    T* tmp = s_instance.load(std::memory_order_acquire);

    if (nullptr == tmp)
    {
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
            std::atexit(&Handleton<T>::Destroy);
        }
    }

    return tmp;
}

template<typename T>
void Handleton<T>::Destroy()
{
    T* tmp = s_instance.load(std::memory_order_acquire);

    s_isDestroyed.store(true, std::memory_order_release);
    s_instance.store(nullptr, std::memory_order_release);

    delete tmp;
}

#endif // __HANDLETON__

} // namespace ilrd

#endif // __ILRD_HANDLETON__
