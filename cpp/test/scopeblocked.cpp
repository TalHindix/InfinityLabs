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
    {for (size_t i = 0; i < 10; ++i)
    // {
    //     pthread_create(&threads[i], NULL, NotWork, NULL);
    // }

    // for (size_t i = 0; i < 10; ++i)
    // {
    //     pthread_join(threads[i], NULL);
    // }
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

Mutex m;
int counter = 0;

void* TestIt(void* arg)
{
    (void)arg;

    for (size_t i = 0; i < 1000000 ; i++)
    {
        LockGuard<Mutex> lock(m);
        ++counter;
        cout << "Counter: " << counter << endl;
    }

    return NULL;
}

void* NotWork(void* arg)
{
    (void)arg;

    for (size_t i = 0; i < 1000000 ; i++)
    {
        ++counter;
        cout << "Counter: " << counter << endl;
    }

    return NULL;
}

void ThreadTest()
{
    pthread_t threads[10] = { 0 };

    for (size_t i = 0; i < 10; ++i)
    {
        pthread_create(&threads[i], NULL, TestIt, NULL);
    }

    for (size_t i = 0; i < 10; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    // for (size_t i = 0; i < 10; ++i)
    // {
    //     pthread_create(&threads[i], NULL, NotWork, NULL);
    // }

    // for (size_t i = 0; i < 10; ++i)
    // {
    //     pthread_join(threads[i], NULL);
    // }




}




int main(void)
{
    ThreadTest();


    return 0;
}


