/*****************************************************************************
* Exercise:    Scope Lock
* Date:        24/11/2025
* Developer:   Tal Hindi
* Reviewer:    Ben Dabush
* Status:      Waiting for review~
*****************************************************************************/

#include <iostream>  /* printf */
#include <pthread.h> /* pthread */

using namespace std;

class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_init(&m_mutex,0);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    void Lock()
    {
        pthread_mutex_lock(&m_mutex);
    }

    void UnLock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

private:

    Mutex(const Mutex&);
    Mutex& operator=(const Mutex&);

    pthread_mutex_t m_mutex;
};


template <typename Lockable>
class LockGuard
{
public:
    explicit LockGuard(Lockable& lock) : m_lock(lock)
    {
        m_lock.Lock();
    }

    ~LockGuard()
    {
        m_lock.UnLock();
    }

private:

    LockGuard(const Lockable&);
    LockGuard& operator=(const Lockable&);

    Lockable& m_lock;
};

int main(void)
{
    Mutex m;
    LockGuard<Mutex> mutex(m);
    return 0;
}


