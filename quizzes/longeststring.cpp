#include <string>
#include <iostream>
#include <unordered_set>
#include <algorithm>

using namespace std;

int LongestString(string str)
{
    unordered_set<char> window;
    int left = 0;
    int max_length = 0;

    for (int right = 0; right < str.length(); right++)
    {
        while(window.count(str[right]))
        {
            window.erase(str[left]);
            ++left;
        }

        window.insert(str[right]);
        max_length = max(max_length, right - left + 1);
    }
    
    return max_length;
}

int main()
{
    string str = "abcabcbb";
    int longeststr = LongestString(str);
    cout << "Longest string size: " << longeststr << endl;
}

