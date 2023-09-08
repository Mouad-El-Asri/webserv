#include "../includes/utils.hpp"

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
