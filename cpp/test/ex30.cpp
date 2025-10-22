/*****************************************************************************
 * Exercise:    ex30
 * Date:        21/10/2025
 * Developer:   Tal Hindi
 * Reviewer:
 * Status:      In Progress
 *****************************************************************************/

class X
{
public:
    X() : m_id(++s_cntr)
    {
    }

    int GetId()
    {
        std::cout << m_id << std::endl;
        return m_id;
    }

private:
    int m_id;
    static int s_cntr;
};

int X::s_cntr = 0;

int main()
{
    X x1;
    X x2;

    x1.GetId();
    x2.GetId();

    return 0;
}

/*

a. What does static on a data member imply?
   The variable belongs to the class itself, not to any specific object.
   All objects share a single copy of this variable.

b. What is the meaning of the line starting with int X::? Why do we need it?
Where should we put this line? Meaning: Definition and memory allocation for the
static member variable. Why: Declaration in class doesn't allocate memory.
Without it: linker error. Where: In the .cpp file (NOT in header).

c. Notice our coding convention for static data members.
   Prefix s_ for static members (e.g., s_cntr).

d. Why "static" again? What do global static variables, local static, and class
static have in common? Static storage duration: created once and persists until
program ends. Lifetime: entire program execution.

e. Investigate and research static member functions.
   Functions that belong to the class, not to any object. No 'this' pointer.

f. What can you do with a static member function that is impossible with a
regular method? Call it WITHOUT an object - using only the class name:
   X::StaticFunc(); // no object needed

g. What does a regular member function do that a static member function cannot?
   - Access instance variables (m_id, m_name, etc.)
   - Access 'this' pointer

h. What are the two different syntaxes used to call static member functions?
   X::StaticFunc();    // via class (preferred)
   x1.StaticFunc();    // via object (confusing, not recommended)

i. Can static member functions access private member variables / functions of an
object? YES for static members (like s_cntr). NO for instance members (like
m_id) without an object reference. If passed an object as parameter, it CAN
access that object's private members: static void Print(const X& obj) {
std::cout << obj.m_id; } // legal

*/