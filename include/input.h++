#pragma once

#include <iostream>
#include <limits>
#include <string>

namespace input
{
inline bool readInteger(const char *prompt, int &value, int minimum = 1)
{
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (value >= minimum)
            {
                return true;
            }
            std::cout << "Please enter a number greater than or equal to "
                      << minimum << ".\n";
        }
        else
        {
            if (std::cin.eof())
            {
                return false;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid number. Please try again.\n";
        }
    }
}

inline bool readText(const char *prompt, std::string &value)
{
    while (true)
    {
        std::cout << prompt;
        if (!std::getline(std::cin, value))
        {
            return false;
        }
        if (!value.empty())
        {
            return true;
        }
        std::cout << "This value cannot be empty. Please try again.\n";
    }
}
}
