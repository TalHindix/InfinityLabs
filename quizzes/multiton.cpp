#include <iostream>
#include <map>

class MyClass
{
public:
    int GetX()
    {
        return m_x;
    }
private:
    int m_x;
};

template<typename T>
class Multiton
{
public:
    static T* GetInstance(int index)
    {
        auto iter = m_map.find(index);

        if(iter == m_map.end())
        {
            T* new_instance = new T();
            m_map[index] = new_instance;
            return new_instance;
        }

        return iter->second;
    }

    Multiton(const Multiton& other) = delete;
    Multiton& operator=(const Multiton& other) = delete;

private:
    Multiton();
    ~Multiton() = default;

    static std::map<int,T*> m_map;
};

template<typename T>
std::map<int,T*> Multiton<T>::m_map; 

int main()
{
    MyClass* instance = Multiton<MyClass>::GetInstance(1);
    std::cout << instance << std::endl;

    return 0;
}