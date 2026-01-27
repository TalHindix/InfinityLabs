#include <iostream>
using namespace std;

string SmallestNum(string num, int k)
{
    string st;
    for (auto c : num)
    {
        while(!st.empty() && k > 0 && st.back() > c)
        {
            st.pop_back();
            --k;
        }
        st.push_back(c);
    }

    while(!st.empty() && k > 0)
    {
        st.pop_back();
        --k;
    }

    size_t i = 0;
    while(i < st.size() && st[i] == '0') ++i;

    if(i == st.size()) return "0";

    return st.substr(i);
}

int main()
{
    string num = "10";
    int k = 2;

    string smallest_number = SmallestNum(num,k);

    cout << smallest_number << endl;
}