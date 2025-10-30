/*****************************************************************************
* Exercise:    ex29
* Date:        21/10/2025
* Developer:   Tal Hindi
* Reviewer:     
* Status:      In Progress
*****************************************************************************/

#include <iostream>

class X
{
public:
    explicit X();
    explicit X(int a_);
    explicit X(int a_, int b_);

    operator int() const;
    void Print() const;

private:
    int m_a;
}; 

X::X() : m_a(0) { }
X::X(int a_) : m_a(a_) { }
X::X(int a_, int b_) : m_a(a_ + b_) { }

X::operator int()const 
{ 
    return m_a;
}

void X::Print() const
{ 
    std::cout << "X::Print() " << m_a << std::endl;
}

void Fifi(X x_)
{
    std::cout << "Fifi() " << x_ << std::endl;
    x_.Print();
}


int main()
{
    X x1(7);

    Fifi(x1);     // (1)
    Fifi(X(7));   // (2)
 //   Fifi(9);      // (3)
     Fifi(X(3, 4)); // (4)

    return x1 * 3; //(5)
}


// m. Explain the syntax of line (5) including type / value / side-effect.
//     Type: int
//     Value: x1.operator int() * 3 = 7 * 3 = 21
//     Side-effect: Calls conversion operator on x1
/*
b. Explain the syntax of line (2) including type / value / side-effect.
    Type: X (temporary object)
    Value: m_a = 7
    Side-effect: Ctor called, object created, copied/moved to Fifi, then destroyed

c. Uncomment line (3). What happens? What did the writer of line (3) expect would happen?
    With explicit: Compilation error - implicit conversion blocked
    Without explicit: Works - compiler implicitly converts 9 → X(9)
    Writer expected: Implicit conversion to work

d. What is different between calling Fifi in (1) (2) and (3)? Use the debugger.
    (1) - Passes existing object (copy/move from x1)
    (2) - Creates temporary explicitly, then passes it
    (3) - Compiler creates temporary from int implicitly (if no explicit)

e. What happens if you uncomment (4)? Can you make it work?
    Error: Can't pass two args directly to function expecting X
    Fix: Fifi(X(3, 4));

f. What does the explicit keyword mean on the default Ctor?
    Prevents: X x = {};  or  X x{};  (aggregate initialization)

g. What does the explicit keyword mean on a Ctor with one argument?
    Prevents implicit conversion: Fifi(7) or X x = 7;

h. What does the explicit keyword mean on a Ctor with two arguments or more?
    Prevents: X x = {3, 4}; (C++11 list-initialization)

i. What Ctors do you need to prefix with explicit? Why?
    All by default, unless implicit conversion is intentional part of API
    Prevents unintended conversions that hide bugs

j. Find examples where you would want conversion Ctors.
    std::string(const char*) - convenient string literals
    std::complex<double>(double) - real numbers as complex
    std::vector<int>(size_t) - size constructor

k. There are 3 different ways to write the explicit temporary creation in line (2). Find the other two.
    X(7)      // Functional cast
    X{7}      // Uniform initialization (C++11)
    (X)7      // C-style cast (don't use!)

l. Why do we have 3 ways? Which one should we use? Why?
    Historical reasons (C legacy + C++11 modernization)
    Use: X{7} (modern, prevents narrowing) or X(7) (traditional, clear)
    Avoid: C-style casts

m. Explain the syntax of line (5) including type / value / side-effect.
    Type: int
    Value: x1.operator int() * 3 = 7 * 3 = 21
    Side-effect: Calls conversion operator on x1

n. Why doesn't the method operator int() have a return type?
    Syntax of conversion operators - return type IS the operator name

o. Find examples of where you would like conversion operators.
    operator bool() - smart pointers, streams (if (ptr), if (cin))
    operator std::string() - custom string-like classes
    Iterators converting to pointers

p. Why is this syntactic sugar?
    Instead of x.ToInt(), you write int(x) or use x in int context
    Makes custom types behave like built-in types

q. Change Fifi() to accept a const X&. Does the code still work? How? Why?
    Yes - temporaries can bind to const references
    Extends temporary lifetime for duration of reference

r. Change Fifi() to accept a X&. Does the code still work? How? Why?
    No - temporaries cannot bind to non-const references
    (1) works if x1 is non-const, (2) and (3) fail

s. Find all the differences in meaning, usage, misusage, readability, conversions, performance, etc.
    X x_       - Copy created, modifiable, accepts temps, overhead for large types
    X& x_      - No copy, modifiable, NO temps, requires lvalue
    const X& x_ - No copy, read-only, accepts temps, MOST EFFICIENT for read-only
    const X x_  - Copy created, read-only, rare (redundant const)
    X* x_      - No copy, nullable, modifiable, pointer semantics 
    const X* x_ - No copy, nullable, read-only
    X* const x_ - Pointer itself is const, object modifiable

t. When should you use each?
    const X& - Default for read-only parameters (efficient, flexible)
    X&       - When you need to modify the argument (out-parameter)
    X        - Small types (int, char) or when copy needed
    X*       - When nullptr is valid or C-style APIs
    const X* - Read-only + nullable
    Smart ptrs - Ownership transfer/sharing
*/