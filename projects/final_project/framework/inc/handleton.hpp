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

    ~Handleton() = default;
    Handleton(const Handleton& other) = delete;
    Handleton& operator=(const Handleton& other) = delete;
    
private:
    Handleton() = default;
    
    static std::atomic<T*> s_instance;
    static std::mutex s_mutex;
    static std::atomic<bool> s_destroyed;

}; // class Handleton


template<typename T>
std::atomic<T*> Handleton<T>::s_instance(nullptr);

template<typename T>
std::mutex Handleton<T>::s_mutex;

template<typename T>
std::atomic<bool> Handleton<T>::s_destroyed(false);



#ifdef __HANDLETON__

template<typename T>
T* Handleton<T>::GetInstance()
{
    T* tmp = s_instance.load(std::memory_order_acquire);

    if (nullptr == tmp)
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        tmp = s_instance.load(std::memory_order_relaxed);

        if (nullptr == tmp)
        {
            if (s_destroyed.load(std::memory_order_acquire))
            {
                std::abort();
            }

            tmp = new T();
            s_instance.store(tmp, std::memory_order_release);

            std::atexit([]() 
            {
                delete s_instance.load(std::memory_order_acquire);
                s_instance.store(nullptr, std::memory_order_release);
                s_destroyed.store(true, std::memory_order_release);
            });
        }
    }

    return tmp;
}

#endif // __HANDLETON__

} // namespace ilrd

#endif // __ILRD_HANDLETON__