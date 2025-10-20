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

ostream& operator<<(ostream& os_, const X& x_);

class X
{
public:
    bool operator==(const X& o_) const { return m_a == o_.m_a; }
    //Java style inline – not allowed in our coding standard
private:
    friend ostream& operator<<(ostream& os_, const X& x_);
    int m_a;
};

int main()
{
    X x1;
    X x2;

    cout<< " x1+x2:" << x1 + x2 <<
           " x1==x2:" << (x1 == x2) <<
           " x1:" << x1 <<
           " x2:" << x2 << endl;

    return 0;
}

ostream& operator<<(ostream& os_, const X& x_) 
{
    return os_ << x_.m_a;
}


/*
b. Note the differences between member operators vs. global operators. When will we use each?
    Member Operators - Num of param (1) , Access to member data , Implict Conversions (just from the right side )
    Global Operators -  Num of Param (2) , Not have access to member data , Implict Conversions (from both sides .)
c. Investigate all the syntax used in this example. Write the type / side-effect / value of every expression in main().
    
d. Why do we need the line class X;at the beginning?

e. What is the purpose of the line starting with friend? Remove it to investigate.

f. Will non-member operators always be friends? Why?

g. Find a way to implement operator<< without it being a friend.

h. How does friend *play* with public and private? Does it break encapsulation?

i. In order for the operator<<() function to be able to access m_a in this case, should we use a friend attribute, or supply a GetA() getter method? Why?

j. Return the friend line and remove the forward declaration of operator<<. What happens? Why?

k. Where in the class body should the friend declaration be? What does this depend on?

l. Implement some more operators. What did you learn?

m. When should you use operators vs. regular functions?

n. Why do we call this "syntactic sugar"?
*/