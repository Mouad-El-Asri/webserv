#include "../includes/check_directives.hpp"

void	checkServerName(Directives &directives, std::istringstream &iss)
{
	std::string value;

	while (std::getline(iss, value, ' '))
	{
		if (!(isOnlyWhitespaces(value)) && !(value.empty()))
		{
			value = trimSpaces(value);
			if (!(isAlphanumeric(value)) && !(isIPAddress(value)) && !(isDomainName(value)))
				throw std::runtime_error("The server_name directive is not valid.");
			directives.setServerName(value);
		}
	}
	if (directives.getServerNames().empty())
		throw std::runtime_error("The server_name directive value is empty or contains only whitespaces.");
}

void	checkListen(Directives &directives, std::istringstream &iss)
{
	std::string value;

	std::getline(iss, value);
	if (value.empty() || isOnlyWhitespaces(value))
		throw std::runtime_error("The listen directive value is empty or contains only whitespaces.");
	value = trimSpaces(value);
	if (containsWhitespace(value))
		throw std::runtime_error("The listen value contains whitespaces.");
	std::vector<std::string> result;
	splitString(value, result);
	if (!isNum(result[1]))
		throw std::runtime_error("The listen port is not a valid number.");
	if (!(isAlphanumeric(result[0])) && !(isIPAddress(result[0])) && !(isDomainName(result[0])))
		throw std::runtime_error("The listen directive host is not valid.");
	directives.setListen(result[0], atoi(result[1].c_str()));
}

void	checkRoot(Directives &directives, std::istringstream &iss)
{
	std::string path;

	std::getline(iss, path);
	if (path.empty() || isOnlyWhitespaces(path))
		throw std::runtime_error("The root path is empty or contains only whitespaces.");
	path = trimSpaces(path);
	if (containsWhitespace(path))
		throw std::runtime_error("The root path contains whitespaces.");
	if (!(isDirectory(path.c_str())))
		throw std::runtime_error("The root folder doesn't exist.");
	directives.setRoot(path);
}

void	checkIndex(Directives &directives, std::istringstream &iss)
{
	std::string index;

	while (std::getline(iss, index, ' '))
	{
		if (!(isOnlyWhitespaces(index)) && !(index.empty()))
		{
			index = trimSpaces(index);
			directives.setIndex(index);
		}
	}
	if (directives.getIndex().empty())
		throw std::runtime_error("The index directive is empty or contains only whitespaces.");
}

void	checkMaxBodySize(Directives &directives, std::istringstream &iss)
{
	std::string size;

	std::getline(iss, size);
	if (size.empty() || isOnlyWhitespaces(size))
		throw std::runtime_error("The client max body size is empty or contains only whitespaces.");
	size = trimSpaces(size);
	if (containsWhitespace(size))
		throw std::runtime_error("The client max body size contains whitespaces.");
	std::string numericPart = size;
	std::string suffix;
    size_t suffixPos = numericPart.find_first_not_of("0123456789");
    if (suffixPos != std::string::npos)
	{
		suffix = numericPart.substr(suffixPos);
        numericPart = numericPart.substr(0, suffixPos);
		if (numericPart == "")
			throw std::runtime_error("The max body size is not valid.");
		if (suffix == "K" || suffix == "KB")
			directives.setMaxBodySizeInBytes(atoi(numericPart.c_str()) * 1024);
		else if (suffix == "M" || suffix == "MB")
			directives.setMaxBodySizeInBytes(atoi(numericPart.c_str()) * 1024 * 1024);
		else if (suffix == "G" || suffix == "GB")
			directives.setMaxBodySizeInBytes(atoi(numericPart.c_str()) * 1024 * 1024 * 1024);
		else
			throw std::runtime_error("The max body size suffix is not valid.");
	}
	else
		directives.setMaxBodySizeInBytes(atoi(numericPart.c_str()));
}

void	checkErrorPages(Directives &directives, std::istringstream &iss)
{
	std::string value;

	std::getline(iss, value);
	if (value.empty() || isOnlyWhitespaces(value))
		throw std::runtime_error("The error_page directive value is empty or contains only whitespaces.");
	value = trimSpaces(value);
	std::vector<std::string> result;
	std::string substring;
	substring = "";
    for (unsigned int i = 0; i < value.size(); i++)
	{
        if (value[i] == ' ')
		{
			result.push_back(substring);
			substring = "";
        }
		else
			substring += value[i];
    }
	if (!substring.empty())
        result.push_back(substring);
	if (result.size() != 2)
		throw std::runtime_error("The error_page directive syntax is invalid.");
	else if (!isNum(result[0]))
		throw std::runtime_error("The error_page status code is not valid.");
	else if (atoi(result[0].c_str()) < 100 || atoi(result[0].c_str()) > 599)
		throw std::runtime_error("The error_page status code range is not valid.");
	else if (access(result[1].c_str(), F_OK) != 0)
		throw std::runtime_error("The error_page file doesn't exist.");
	else if (access(result[1].c_str(), R_OK) != 0)
		throw std::runtime_error("The error_page file is not readable.");
	directives.setErrorPage(atoi(result[0].c_str()), result[1]);
}

void	checkAutoIndex(Directives &directives, std::istringstream &iss)
{
	std::string value;

	std::getline(iss, value);
	if (value.empty() || isOnlyWhitespaces(value))
		throw std::runtime_error("The autoindex directive is empty or contains only whitespaces.");
	value = trimSpaces(value);
	if (containsWhitespace(value))
		throw std::runtime_error("The autoindex value contains whitespaces.");
	if (value == "on" || value == "ON")
		directives.setAutoIndex(true);
	else if (value == "off" || value == "OFF")
		directives.setAutoIndex(false);
	else
		throw std::runtime_error("The autoindex value is not valid.");
}
