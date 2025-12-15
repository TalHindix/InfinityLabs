/*****************************************************************************
 * Exercise:    Reactor
 * Date:        11/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#ifndef __ILRD_REACTOR__
#define __ILRD_REACTOR__

#include <vector> // std::vector
#include <memory> // std::shared_ptr
#include <functional> //std::function

namespace ilrd
{
 
class Reactor
{
public:
    enum Mode
    {
        READ,
        WRITE
    };

    using FdPair = std::pair<int, Reactor::Mode>;
    using CallBack = std::function<void(int, Mode)>;

    class IListener
    {
    public:
        virtual ~IListener() = default;
        virtual std::vector<FdPair> Listen(const std::vector<FdPair>& set) = 0;
    };
    
    explicit Reactor(const std::shared_ptr<IListener> listener);
    ~Reactor() noexcept = default;
    
    Reactor(const Reactor& other) = delete;
    Reactor& operator=(const Reactor& other) = delete;
    Reactor(const Reactor&& other) = delete;
    Reactor& operator=(const Reactor&& other) = delete;
    
    void Add (int fd, Mode mode, CallBack callback);
    void Remove (int fd, Mode mode);                  
    //run is a blocking function, and it returns when stop is called
    void Run ();
    void Stop ();

    
private:
    class Hasher
    {
    public:
        std::size_t operator()(const FdPair& pair) const noexcept
        {
            std::size_t h1 = std::hash<int>{}(pair.first);
            std::size_t h2 = std::hash<int>{}(pair.second);
            return h1 ^ (h2 << 1);
        }
        // shimon's example: return std::hash<int>{}(p.first * (1 + (10 * p.second)));
    };
    
    std::unordered_map<FdPair, CallBack, Hasher> m_callbacks;
    const std::shared_ptr<IListener> m_listener;
    bool m_is_running;
    
    
}; // class Reactor
} // ilrd

#endif //__ILRD_REACTOR__