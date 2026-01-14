#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> v;

void AddNumber(int num)
{
    v.push_back(num);
}

float getMedian()
{
    if(v.empty()) return 0;

    std::sort(v.begin(),v.end());

    
}

int main()
{
    AddNumber(1);
    AddNumber(2);

    std::cout << getMedian() << std::endl;



    return 0;
}