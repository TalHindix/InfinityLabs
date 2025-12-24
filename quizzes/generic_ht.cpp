#include <iostream>
#include <list>

template<typename K , typename V>
class HashTable
{
private:
    int m_capacity;
    std::list<std::pair<K,V>>* m_table;

    int HashFunction(K key)
    {
        return static_cast<long long>(key) % m_capacity; 
    }

public:

    explicit HashTable(int capacity) : m_capacity(capacity)
    {
        m_table = new std::list<std::pair<K,V>>[m_capacity];
    }

    ~HashTable()
    {
        delete[] m_table;
    }

    bool Insert(K key, V val)
    {
        int index = HashFunction(key);
        for(auto& pair : m_table[index])
        {
            if (pair.first == key)
            {
                pair.second = val;
                return true;
            }
        }
        
        m_table[index].push_back(std::make_pair(key,val));     
        return true;
    }

    bool Remove(K key)
    {
        int index = HashFunction(key);
        
        for(
            auto it = m_table[index].begin(); it != m_table[index].end(); ++it)
        {
            if(it->first == key)
            {
                m_table[index].erase(it);
                return true;
            }
        }

        return false;
    }

    V* Find(K key)
    {
        int index = HashFunction(key);

        for(auto& pair: m_table[index])
        {
            if(pair.first == key)
            {
                return &pair.second;
            }
        }

        return nullptr;
    }
};

int main()
{   
    HashTable<int,int> h1(10);
    h1.Insert(5,10);
    h1.Insert(15,3);
    return 0;
}