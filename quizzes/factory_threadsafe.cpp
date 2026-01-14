#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <map>

template <typename T, typename K, typename D>
class Factory
{
public:
    using CreateFunc = std::function<T(D)>;
private:
    Factory();
    ~Factory() = default;
    Factory& operator=(const Factory& other) = delete;

    std::map<K, CreateFunc> m_creator;
    std::mutex m_mutex;
    
}


int main()
{
    return 0;
}