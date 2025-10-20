#include "mylib.hpp"

namespace ilrd {

int Add(int a, int b) { return a + b; }

extern "C"
{
    int AddNumbers(int a, int b)
    {
        return ilrd::Add(a, b);
    }
}

} // namespace ilrd