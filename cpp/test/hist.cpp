/*****************************************************************************
 * Exercise:    histo
 * Date:        30/10/2025
 * Developer:   Tal Hindi
 * Status:      Finished
 *****************************************************************************/

#include <iostream> // std::cin
#include <map>      // std::map
#include <string>   // std::string
#include <vector>   // std::vector

int main()
{
    std::string userInput;
    std::vector<std::string> vector;
    std::map<std::string, std::size_t> counts;

    std::cout << "Enter strings: (a dot will terminate the program) \n";
    while (std::getline(std::cin, userInput) && "." != userInput)
    {

        if (1 == ++counts[userInput])
        {
            vector.push_back(userInput);
        }
    }

    std::cout << "\n --------------The Program is terminated successfuly-------------------- " << std::endl;

    for (size_t i = 0; i < vector.size(); ++i)
    {
        if (counts[vector[i]] > 1)
        {
            std::cout << (counts[vector[i]]) << " x " << vector[i] << std::endl;
        }
        else
        {
            std::cout << vector[i] << std::endl;
        }
    }

    return 0;
}