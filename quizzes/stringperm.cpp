#include <iostream>
#include <string>
#include <vector>

bool containPerm(std::string s1, std::string s2);

int main()
{
    std::string s1 = "ab";
    std::string s2 = "eidbaooo";

    bool contain = containPerm(s1,s2);
    std::cout << contain << std::endl;

    return 0;
}


bool containPerm(std::string s1, std::string s2)
{
    int n = s1.length();
    int m = s2.length();

    if (s1.length() > s2.length())
        return false;

    std::vector<int> need(26,0);
    std::vector<int> window(26,0);
    
    for(char c : s1)
    {
        need[c - 'a']++;
    }

    for(int i = 0; i < n; ++i)
    {
        window[s2[i] - 'a']++;
    }

    if(need == window)
    {
        return true;
    }

    for(int right = n; right < m; ++right)
    {
        window[s2[right] - 'a']++;

        window[s2[right - n] - 'a']--;

        if(window == need)
        {
            return true;
        }
    }

    return false;
}