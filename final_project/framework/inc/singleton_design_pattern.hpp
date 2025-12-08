/*****************************************************************************
 Exercise:    Singleton Design Pattern
 Date:        06/12/2025
 Developer:   Tal Hindi
 Reviewer:    
 Status:      
 *****************************************************************************/

 #ifndef _ILRD_SINGLETON
 #define _ILRD_SINGLETON

 #include <iostream>
 #include <atomic>
 #include <mutex>
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
            std::lock_guard<std::mutex> lock(s_mutex);
            tmp = s_instance.load(std::memory_order_relaxed);
            if (nullptr == tmp)
            {
                tmp = new T();
                s_instance.store(tmp, std::memory_order_release);
            }
        }
        return tmp;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() = default;
    ~Singleton() = default;
private:
    static std::atomic<T*> s_instance;
    static std::mutex s_mutex;
};

template<typename T>
std::atomic<T*> Singleton<T>::s_instance{nullptr};

template<typename T>
std::mutex Singleton<T>::s_mutex;

class Derived : public Singleton<Derived>
{
    friend class Singleton<Derived>;
public:
    void DoWork();
private:
};

} // ilrd 




 #endif // _ILRD_SINGLETON