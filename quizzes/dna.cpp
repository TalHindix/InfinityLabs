#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
using namespace std;

list<string> findRepeatedSequences(string s)
{
    list<string> list;
    size_t dna_len = 10;

    unordered_map<string,int> frequency;

    for (size_t i = 0; i + dna_len < s.length() ; ++i)
    {
        string sub = s.substr(i,dna_len);
        frequency[sub]++;

        if(frequency[sub] == 2)
        {
            list.push_back(sub);
        }
    }
    
    return list;
}
int main()
{
    string s = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT";
    list<string> dna_seq = findRepeatedSequences(s);
    
    for(auto s : dna_seq)
    {
        cout << s << endl;
    }

    return 0;
}