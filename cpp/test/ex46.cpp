#include <iostream>

template<class T>
class Stack
{
private:
    T* stk;
    int top;
    int size;

public:
    Stack(int sz)
    {
        size = sz;
        top = -1;
        stk = new T[size];
    }

    void Push(T x);
    T Pop();
};

template<class T>
void Stack<T>::Push(T x)
{
    if (top == size - 1)
    {
        std::cout << "Stack is FULL" << std::endl;
    }
    else
    {
        top++;
        stk[top] = x;
    }
}

template<class T>
T Stack<T>::Pop()
{
    int x = 0;
    if (top == -1)
    {
        std::cout << "Stack Is Empty" << std::endl;
    }
    else
    {
        x = stk[top];
        top--;
    }
    return x;
}

int main(void)
{
    Stack<int> s(10);
    s.Push(3);
    s.Push(5);
    s.Push(2);
    return 0;
}