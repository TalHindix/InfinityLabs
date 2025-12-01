/*****************************************************************************
 * Exercise:    Priority Queue
 * Date:        1/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#ifndef _ILRD_PQ
#define _ILRD_PQ

#include <vector>       // std::vector
#include <queue>        // std::priority_queue
#include <functional>   // std::less
#include <cstddef>      // std::size_t

namespace ilrd
{

template <typename T,
          typename Container = std::vector<T>,
          typename Compare = std::less<T> >
class PriorityQueue
{
public:

    void Push(const T& item)
    {
        m_pq.push(item);
    }

    void Pop()
    {
        m_pq.pop();
    }

    const T& Front() const
    {
        return m_pq.top();
    }

    bool Empty() const
    {
        return m_pq.empty();
    }

    std::size_t Size() const
    {
        return m_pq.size();
    }

private:
    std::priority_queue<T, Container, Compare> m_pq;

};

} // namespace ilrd

#endif /* _ILRD_PQ */
