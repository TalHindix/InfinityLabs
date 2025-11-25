/*****************************************************************************
 * Exercise:    Shared Pointer
 * Date:        24/11/2025
 * Developer:   Tal Hindi
 * Reviewer:    Meir
 * Status:
 *****************************************************************************/

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
    template <typename U>
    SharedPtr& operator=(const SharedPtr<U>& other);

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
template<typename U>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<U>& other)
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

class Base
{
public:
    explicit Base(int val);
    ~Base();
    void Foo();

private:
    int m_val;
};

Base::Base(int val) : m_val(val)
{
    std::cout << "Base Ctor applied" << std::endl;
}

Base::~Base()
{
    std::cout << "Base Dtor applied" << std::endl;
}

void Base::Foo()
{
    std::cout << "Hello" << std::endl;
}

class Derived : public Base
{
public:
    explicit Derived(int val);
    ~Derived();

private:
    int m_val;
};

Derived::Derived(int val) : Base(5), m_val(val)
{
    std::cout << "Derived Ctor applied" << std::endl;
}

Derived::~Derived()
{
    std::cout << "Derived Dtor applied" << std::endl;
}

int main(void)
{
    using ilrd::SharedPtr;
    using std::cout;
    using std::endl;

    cout << "===== Test 1: Empty SharedPtr =====" << endl;
    {
        SharedPtr<Base> p1;
        cout << "RefCount: " << p1.GetRefCount() << endl;
    }

    cout << "\n===== Test 2: Basic construction =====" << endl;
    {
        SharedPtr<Base> p1(new Base(10));
        cout << "RefCount: " << p1.GetRefCount() << endl;
    }

    cout << "\n===== Test 3: Copy construction =====" << endl;
    {
        SharedPtr<Base> p1(new Base(20));
        SharedPtr<Base> p2(p1);
        cout << "RefCount p1: " << p1.GetRefCount() << endl;
        cout << "RefCount p2: " << p2.GetRefCount() << endl;
    }

    cout << "\n===== Test 4: Multiple references leaving scope =====" << endl;
    {
        SharedPtr<Base> p1(new Base(30));
        {
            SharedPtr<Base> p2(p1);
            SharedPtr<Base> p3(p1);
            cout << "RefCount: " << p1.GetRefCount() << endl;
        }
        cout << "After inner scope RefCount: " << p1.GetRefCount() << endl;
    }

    cout << "\n===== Test 6: Derived → Base conversion =====" << endl;
    {
        SharedPtr<Derived> d1(new Derived(7));
        {
            SharedPtr<Base> b1(d1);
            cout << "RefCount d1: " << d1.GetRefCount() << endl;
            cout << "RefCount b1: " << b1.GetRefCount() << endl;
        }
        cout << "After b1 scope RefCount d1: " << d1.GetRefCount() << endl;
    }

    cout << "\n===== Test 8: Assignment releases old object =====" << endl;
    {
        SharedPtr<Base> p1(new Base(10));
        SharedPtr<Base> p2;

        p2 = p1;
        cout << "After p2 = p1, p1 RefCount: " << p1.GetRefCount() << endl;

        p1 = SharedPtr<Base>(new Base(50));
        cout << "After p1 reassigned, p1 RefCount: " << p1.GetRefCount()
             << endl;
        cout << "p2 RefCount (old object): " << p2.GetRefCount() << endl;
    }

    return 0;
}
