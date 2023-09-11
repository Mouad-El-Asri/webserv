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

bool	isIPAddress(const std::string &str)
{
	std::istringstream iss(str);
	std::string segment;

	int	numSegments;
	int num;

	numSegments = 0;
	while (std::getline(iss, segment, '.'))
	{
		if (!(std::istringstream(segment) >> num))
			return false;

		if (num < 0 || num > 255)
			return false;

        numSegments++;
    }

    return (numSegments == 4);
}

bool isValidDomainSegment(const std::string &segment)
{
    if (segment.empty() || segment.length() > 63)
        return (false);

    for (int i = 0; i < segment.length(); i++)
	{
        if (!std::isalnum(segment[i]) && segment[i] != '-')
            return (false);
    }

    if (segment.front() == '-' || segment.back() == '-')
        return (false);

    return (true);
}

bool isDomainName(const std::string &str)
{
    std::vector<std::string>	segments;
    std::string					segment;

    for (int i = 0; i < str.length() ; i++)
	{
		if (str[i] == '.')
		{
            if (!isValidDomainSegment(segment))
                return (false);
            segments.push_back(segment);
            segment.clear();
        }
		else
            segment.push_back(str[i]);
    }

    if (!isValidDomainSegment(segment))
        return (false);

    segments.push_back(segment);

    return (!segments.empty());
}


bool isAlphanumeric(const std::string &str)
{
    for (int i = 0; i < str.length() ; i++)
	{
        if (!std::isalnum(str[i]))
            return (false);
    }
    return (true);
}
