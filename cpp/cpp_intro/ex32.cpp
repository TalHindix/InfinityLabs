/*****************************************************************************
 * Exercise:    ex32
 * Date:        22/10/2025
 * Developer:   Tal Hindi
 * Reviewer:
 * Status:      In Progress
 *****************************************************************************/

#include <iostream>

using namespace std;

class B
{
public:
    B(int a_ = 8) : m_a(a_)
    {
        cout << "B::Ctor" << endl;
    }

    virtual ~B()
    {
        cout << "B::Dtor" << endl;
    }

    // Java style is not allowed in our coding convention
    virtual void Print1() const;
    virtual void Print2() const;
    virtual void Print3() const;

private:
    int m_a;
};

void B::Print1() const
{
    cout << "B::Print1 B::m_a - " << m_a << endl;
}

void B::Print2() const
{
    cout << "B::Print2" << endl;
}

void B::Print3() const
{
    cout << "B::Print3" << endl;
}

class X : public B
{
public:
    X() : m_b(0)
    {
        cout << "X::Ctor" << endl;
    }

    ~X()
    {
        cout << "X::Dtor" << endl;
    }

    virtual void Print1() const
    {
        cout << "X::Print1 X::m_b - " << m_b << endl;
        B::Print1();
        cout << "X::Print1 end" << endl;
    }

    void Print2() const
    {
        cout << "X::Print2" << endl;
    }

private:
    int m_b;
};

int main()
{
    B* b1 = new B;
    B* b2 = new X;

    cout << endl << "main b1:" << endl;
    b1->Print1();
    b1->Print2();
    b1->Print3();

    cout << endl << "main b2:" << endl;
    b2->Print1();
    b2->Print2();
    b2->Print3();

    X* xx = static_cast<X*>(b2);
    cout << endl << "main xx:" << endl;
    xx->Print2();
    b2->Print2();

    delete b1;
    delete b2;

    return 0;
}

/*
b. How can we initialize B* with address of X? What's it good for?
Public inheritance = "is-a" relationship. X is-a B.
Good for polymorphism - write code once for base class, works with all derived
classes.

c. What's called when b2->Print3() executes?
B::Print3() - X doesn't override it, so base version is called.

d. What implicit conversions does public inheritance add?
- Derived* to Base*
- Derived& to Base&
- Derived object to Base const&

e. What can static_cast do?
- Convert numbers (int to double, etc)
- Convert pointers up and down inheritance hierarchy
- Call explicit constructors
- Remove const (dangerous)

f. What's interesting about X::Print1() implementation?
It calls B::Print1() inside using scope resolution.
Overriding doesn't delete base version - you can still call it.

g. Remove default parameter from B Ctor. What happens? Solution?
Won't compile - X() tries to call B's default constructor.
Fix: Add to X constructor: X() : B(8), m_b(0)

h. Remove virtual from B::Print2(). What happens?
Static binding instead of dynamic binding.
b2->Print2() calls B::Print2() instead of X::Print2().
Polymorphism is broken.

i. What's wrong with xx->Print2() and b2->Print2() output?
xx->Print2() calls X version (static binding through X*)
b2->Print2() calls B version (Print2 not virtual)
Same object, different behavior = bad design.

j. Would we ever want this behavior?
No. Same object should behave the same way regardless of pointer type.

k. Remove virtual from B's Dtor. What happens? Why wrong?
delete b2 only calls B::~B(), not X::~X().
X's destructor never runs = memory leak.
Rule: Always make base class destructor virtual.

l. What do differences in X::Print1() vs X::Print2() declarations change?
Nothing important. Both override if base is virtual.
virtual keyword in X is just documentation.

m. How's virtual mechanism implemented?
vtable = array of function pointers (one per class)
vptr = hidden pointer in each object pointing to its vtable
When you call obj->func(), it looks up func in the vtable through vptr.
*/