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

namespace ilrd
{

// template<typename T>   
// class Singleton
// {
// public:
//     ~Singleton();
//     Singleton(const Singleton& obj) = delete;
//     static T* GetInstance();
    
// private:
//     Singleton();
//     static std::mutex lock;
//     static T* instance_ptr;
// }; // class Singleton

template<typename T>
class Singleton
{
public:
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
protected:
    Singleton()= default;
    ~Singleton() = default;
};

class Derived : public Singleton<Derived>
{
    friend class Singleton<Derived>;
public:
    void DoWork();
private:
    Derived() = default;
};

} // ilrd 




 #endif // _ILRD_SINGLETON