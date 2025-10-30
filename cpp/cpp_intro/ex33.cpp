#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

class Tracker
{
public:
    Tracker(const string& name_) : m_name(name_)
    {
        cerr << "Tracker Ctor: " << m_name << " created" << endl;
    }
    
    ~Tracker()
    {
        cerr << "Tracker Dtor: " << m_name << " destroyed" << endl;
    }

private:
    string m_name;
};

struct BadDog : public runtime_error
{
    BadDog(const string& s_ = "This is a bad dog") : runtime_error(s_)
    {
    }
};

void Fifi()
{
    throw BadDog("bad pup");
    cerr << "Fifi() after throw" << endl;
}

void Foo()
{
    Tracker fooTracker("Foo");
    Fifi();
    cerr << "Foo() after Fifi()" << endl;
}

void Bar()
{
    Foo();
    cerr << "Bar() after Foo()" << endl;
}

int main()
{
    try
    {
        Bar();
    }
    catch (bad_alloc&)
    {
        cerr << "Out of memory! exiting.";
        exit(2);
    }
    catch (BadDog& b)
    {
        cerr << "Bad dog exception: " << b.what();
        exit(3);
    }
    catch (exception& r)
    {
        cerr << "unknown exception: " << r.what();
        exit(4);
    }
    catch (...)  // j. catch-all block
    {
        cerr << "Caught unknown type!";
        exit(5);
    }
    return 0;
}

/*
b. Use the debugger to investigate Ctors and Dtors.
// Using debugger, you'll see that destructors are called during stack unwinding
// in reverse order of construction, even though functions don't complete normally.

c. Note what gets printed out, and what does not. Why?
// Printed: "Bad dog exception: bad pup"
// NOT printed: All cerr statements AFTER the throw
// Why: throw immediately transfers control to catch handler, skipping all code after it

d. What is exception?
// It's the base class for all standard C++ exceptions defined in <exception>
// Provides virtual what() method for polymorphic error descriptions

e. What is logical_error?
// It's a derived class from <exception> that represents errors in program logic
// (e.g., invalid_argument, domain_error, out_of_range)
// NOT compile-time errors - these are runtime errors that represent logic mistakes

f. What is bad_alloc?
// Exception thrown by 'new' when memory allocation fails
// Also derived from exception

g. Remove the catch(BadDog&) and its block. Run it this way and explain the output.
// Output: "unknown exception: bad pup"
// BadDog is derived from runtime_error, which is derived from exception
// The catch(exception&) block catches it through polymorphism

h. Change the order of the catch blocks. Does the order have any significance?
// Yes! If we put the general catch(exception&) first, it will catch ALL derived exceptions
// The more specific catches below become unreachable (dead code)
// Always order: most specific (derived) → most general (base)

i. Add a class that prints in its Ctor and Dtor, and add a local variable of
that class to the function Foo() before the call to Fifi(). Use the debugger to
investigate the results. What did you learn?
// OUTPUT WITH TRACKER:

// Tracker Ctor: Foo created
// Tracker Dtor: Foo destroyed    ← Automatic cleanup
// Bad dog exception: bad pup
//
// LEARNED: Stack unwinding is deterministic and orderly.
// All local objects get destroyed in reverse order, guaranteeing cleanup (RAII).
// Resources are ALWAYS released, even during exceptions!

j. Change the throw line to throw something else. Add a catch(...) block. 
Why do the catch blocks take the exception objects by reference? 
Experiment and research.

// REASONS TO CATCH BY REFERENCE:
// 1. Prevents SLICING: Catching by value would slice derived exception objects
//    to base type, losing derived class information and polymorphism
// 2. EFFICIENCY: No copying of exception objects
// 3. POLYMORPHISM: References preserve dynamic type, so what() calls correct version
// 4. CONSISTENCY: Matches semantics of polymorphic behavior
//
// EXPERIMENT:
// throw 42;           // caught by catch(...)
// throw "string";     // caught by catch(...)
// throw BadDog();     // caught by catch(BadDog&) or catch(exception&)
//
// catch(...) is a catch-all that matches ANY type, including non-exception objects
// It's useful as a last resort but provides no information about what was thrown

// EXAMPLE OF SLICING PROBLEM:
// catch(exception e)  // BY VALUE - BAD!
// {
//     // If BadDog was thrown, it gets sliced to exception
//     // e.what() might not call BadDog's version
//     // Derived class data is lost!
// }
*/