/*****************************************************************************
 * Exercise:    ex28
 * Date:        20/10/2025
 * Developer:   Tal Hindi
 * Reviewer:
 * Status:      In Progress
 *****************************************************************************/

#include <iostream> // cout

using namespace std;

class X; // Forward declartion. to use int operator+ func .

int operator+(const X& x1_, const X& x2_)
{
    return 7;
}

// ostream& operator<<(ostream& os_, const X& x_);

class X
{
public:
    bool operator==(const X& o_) const
    {
        return m_a == o_.m_a;
    }
    // Java style inline – not allowed in our coding standard
private:
    friend ostream& operator<<(ostream& os_, const X& x_);
    int m_a;
};

int main()
{
    X x1;
    X x2;

    cout << " x1+x2:" << x1 + x2 << " x1==x2:" << (x1 == x2) << " x1:" << x1
         << " x2:" << x2 << endl;

    return 0;
}

ostream& operator<<(ostream& os_, const X& x_)
{
    return os_ << x_.m_a;
}

/*
b. Member operators vs. global operators:
   Member: 1 parameter, has access to private members, implicit conversion only
on right side Global: 2 parameters, no member access (unless friend), implicit
conversion on both sides Use member for: ==, =, +=, [], ->, operations that
modify left operand Use global for: +, <<, binary arithmetic, when left operand
isn't your class

c. Expression types/side-effects/values in main():
   x1 + x2: type=int, value=7, side-effect=none
   x1 == x2: type=bool, value=undefined (m_a uninitialized), side-effect=none
   x1: type=X&, value=x1, side-effect=outputs m_a to cout
   x2: type=X&, value=x2, side-effect=outputs m_a to cout

d. class X; forward declaration needed because global operator+ uses X before
class definition

e. friend allows operator<< to access private m_a
   Without it: compilation error - cannot access private member

f. No. Only need friend if accessing private/protected members

g. Add public getter GetA() in class, use x_.GetA() in operator

h. friend doesn't break encapsulation - it's controlled access granted by the
class

i. Use friend - operator<< is part of X's interface, GetA() would expose m_a
unnecessarily

j. Compilation error - operator<< used before declared
   friend declaration also serves as forward declaration

k. Anywhere in class body (public/private). Convention: in private section

l. (Your findings from implementing operators)

m. Use operators for intuitive operations (==, +, <<)
   Use functions for complex/non-intuitive operations

n. "Syntactic sugar" - operators provide nicer syntax without new functionality
   x1.Add(x2) vs x1 + x2 - same result, cleaner syntax
*/