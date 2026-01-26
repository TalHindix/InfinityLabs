#include <iostream>
using namespace std;

string expand(string s, int left, int right)
{
    while(left >= 0 && right < s.length() && s[left] == s[right])
    {
        --left;
        ++right;
    }

    return s.substr(left + 1,right - left - 1);
}

string longestPalindrome(string s)
{
    string result = "";

    for(int i=0 ; i < s.length(); ++i)
    {
        string odd = expand(s,i,i);
        string even = expand(s,i, i + 1);

        if(odd.length() > result.length()) result = odd;
        if(even.length() > result.length()) result = even;

    }

    return result;
}

int main()
{
    string s1 = "babad";
    string s2 = "cbbd";

    string pali = longestPalindrome(s1);
    cout << pali << endl;

    pali = longestPalindrome(s2);
    cout << pali;

    return 0;
}