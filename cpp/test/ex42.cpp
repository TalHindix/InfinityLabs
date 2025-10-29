class X
{
public:
    X(double d_): m_a(d_)
    {
    }

    virtual ~X()
    {
    }

private:
    X& operator=(const X&);
    double m_a;
};

class Y: public X
{
public:
    Y(double d_, int i_): X(d_), m_b(i_)
    {
    }

private:
    int m_b;
};

int main()
{
    Y y1(0.0, 0), y2(14.2, 5);
    Y y3(0.0, 0), y4(14.2, 5);

    X& x1 = y1;
    X& x2 = y2;

    x1 = x2;
    y3 = y4;

    return 0;
}

/*
a. Use the debugger to look at y1 and y3 after the assignments.
-------------
After x1 = x2;:

y1.m_a = 14.2 (changed)
y1.m_b = 0 (unchanged!)

After y3 = y4;:

y3.m_a = 14.2 (changed)
y3.m_b = 5 (changed)
-----------------
b. What is wrong?
--------
The reference assignment x1 = x2 only copied the X part (slicing). The derived
part m_b wasn't copied.
--------
c. Why did it happen?
-------
The compiler-generated X::operator= only knows about X's members. It has no idea
Y exists. When called through a reference, it performs object slicing - copying
only the base subobject.
-------
d. Why is it so wrong?
----
Partial state corruption. The object y1 is now in an inconsistent state - half
from the old object, half from the new. This violates class invariants and
creates impossible-to-debug scenarios where objects don't match their
construction parameters.
---
e. Who is to blame?
----
The class designer. A polymorphic base class (one with virtual functions) that
allows assignment through base references is a design defect. Virtual destructor
signals "I'm meant to be inherited from" - assignment operators must match that
intent.
---
f. How can we avoid this situation? What is the new habit?
X& operator=(const X&) = delete;
*/