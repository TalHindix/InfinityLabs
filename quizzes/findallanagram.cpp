#include <iostream>
#include <list>
#include <array>

using namespace std;

list<int> FindAllAnagrams(string s, string p)
{
    list<int> list;
    int s_size = s.length();
    int p_size = p.length();
    int start_index = 0;
    int missing = p_size;

    int need[26] = {0};
    for (char c : p) need[c - 'a']++;

    for(int i= 0; i < s_size; ++i)
    {
       int in = s[i] - 'a';
       if(need[in] > 0)
       {
            --missing;
       }
       --need[in];

       if(i - start_index + 1 > p_size)
       {
            int out = s[start_index] - 'a';
            ++need[out];
            if(need[out] > 0)
            {
                ++missing;
            }

            ++start_index;
       }

       if(i - start_index + 1 == p_size )


    }



    return list;
}
int main()
{
    string s = "cbaebabacd";
    string p = "abc";
    list<int> indexs = FindAllAnagrams(s,p);
    return 0;
}