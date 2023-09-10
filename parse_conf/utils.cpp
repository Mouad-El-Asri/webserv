#include "../includes/utils.hpp"
#include <algorithm>

std::string	trimSpaces(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");

    return (str.substr(start, end - start + 1));
}

bool	isOnlyWhitespaces(const std::string& str)
{
    for (std::string::const_iterator it = str.begin(); it != str.end(); it++)
	{
        if (!std::isspace(*it))
            return (0);
    }
    return (1);
}

void removeWhitespaces(std::string& str)
{
    std::string::iterator	it = str.begin();
    while (it != str.end())
    {
        if (std::isspace(*it))
            it = str.erase(it);
		else
        	it++;
    }
}

bool	charIsInString(const std::string& str, const char ch)
{
    return (str.find(ch) != std::string::npos);
}
