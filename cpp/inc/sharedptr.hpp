/*****************************************************************************
 * Exercise:    Shared Pointer
 * Date:        24/11/2025
 * Developer:   Tal Hindi
 * Reviewer:    Baruchi Haimson
 * Status:      Approved
 *****************************************************************************/

#ifndef __ILRD_SHAREDPTR__
#define __ILRD_SHAREDPTR__

#include <cstddef>  // std::size_t
#include <iostream> // std::cout

namespace ilrd
{

template<typename T>
class SharedPtr
{
public:
    explicit SharedPtr(T* ptr = NULL);
    SharedPtr(const SharedPtr& other);
    template <typename U>
    SharedPtr(const SharedPtr<U>& other);
    ~SharedPtr();

    SharedPtr& operator=(const SharedPtr& other);

    T& operator*() const;
    T* operator->() const;
    T* Get() const;
    std::size_t GetRefCount() const;

private:
    template <typename U>
    friend class SharedPtr;

    T* m_ptr;
    std::size_t* m_refcount;
};

template<typename T>
SharedPtr<T>::SharedPtr(T* ptr) : m_ptr(ptr), m_refcount(NULL)
{
    if (NULL != m_ptr)
    {
        m_refcount = new std::size_t(1);
    }
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other)
    : m_ptr(other.m_ptr), m_refcount(other.m_refcount)
{
    if (NULL != m_refcount)
    {
        ++(*m_refcount);
    }
}

template<typename T>
template<typename U>
SharedPtr<T>::SharedPtr(const SharedPtr<U>& other)
    : m_ptr(other.m_ptr), m_refcount(other.m_refcount)
{
    if (NULL != m_refcount)
    {
        ++(*m_refcount);
    }
}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
    if (NULL != m_refcount)
    {
        --(*m_refcount);
        if (0 == *m_refcount)
        {
            delete m_ptr;
            delete m_refcount;
        }
    }
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other)
{
    if (this != &other)
    {
        if (NULL != m_refcount)
        {
            --(*m_refcount);
            if (0 == *m_refcount)
            {
                delete m_ptr;
                delete m_refcount;
            }
        }

        m_ptr = other.m_ptr;
        m_refcount = other.m_refcount;

        if (NULL != m_refcount)
        {
            ++(*m_refcount);
        }
    }

    return *this;
}

template<typename T>
T& SharedPtr<T>::operator*() const
{
    return *m_ptr;
}

template<typename T>
T* SharedPtr<T>::operator->() const
{
    return m_ptr;
}

template<typename T>
T* SharedPtr<T>::Get() const
{
    return m_ptr;
}

template<typename T>
std::size_t SharedPtr<T>::GetRefCount() const
{
    if (NULL == m_refcount)
    {
        return 0;
    }

    return *m_refcount;
}

} // namespace ilrd

#endif /* __ILRD_SHAREDPTR__ */
