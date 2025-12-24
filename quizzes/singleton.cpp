#include <iostream>


template <typename T>
class Singleton
{
private:
    Singleton() = delete;
    ~Singleton() = default;
    
public:
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }

    Singleton& operator=(const Singleton& other) = delete;
    Singleton(const Singleton& other) = delete;

};


int main()
{
    int& obj = Singleton<int>::GetInstance();

    return 0;
}

