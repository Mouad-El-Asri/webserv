#include "../includes/check_location.hpp"

void	checkLocationRoot(Locations &location, std::istringstream &iss)
{
	std::string path;

	std::getline(iss, path);
	if (path.empty() || isOnlyWhitespaces(path))
		throw std::runtime_error("The location root path is empty or contains only whitespaces.");
	path = trimSpaces(path);
	if (containsWhitespace(path))
		throw std::runtime_error("The location root path contains whitespaces.");
	if (!(isDirectory(path.c_str())))
		throw std::runtime_error("The location root folder doesn't exist.");
	location.setRoot(path);
}

void	checkLocationIndex(Locations &location, std::istringstream &iss)
{
	std::string index;

	while (std::getline(iss, index, ' '))
	{
		if (!(isOnlyWhitespaces(index)) && !(index.empty()))
		{
			index = trimSpaces(index);
			location.setIndex(index);
		}
	}
	if (location.getIndex().empty())
		throw std::runtime_error("The location index directive is empty or contains only whitespaces.");
}

void	checkLocationAutoIndex(Locations &location, std::istringstream &iss)
{
	std::string value;

	std::getline(iss, value);
	if (value.empty() || isOnlyWhitespaces(value))
		throw std::runtime_error("The location autoindex directive is empty or contains only whitespaces.");
	value = trimSpaces(value);
	if (containsWhitespace(value))
		throw std::runtime_error("The location autoindex value contains whitespaces.");
	if (value == "on" || value == "ON")
		location.setAutoIndex(true);
	else if (value == "off" || value == "OFF")
		location.setAutoIndex(false);
	else
		throw std::runtime_error("The location autoindex value is not valid.");
}

void	checkLocationAcceptedMethods(Locations &location, std::istringstream &iss)
{
	std::string method;

	while (std::getline(iss, method, ' '))
	{
		if (!(isOnlyWhitespaces(method)) && !(method.empty()))
		{
			method = trimSpaces(method);
			if (method == "GET")
			{
				if (location.getAcceptedMethods()["GET"] == true)
					throw std::runtime_error("The GET method value is alraedy set as accepted method.");
				location.setAcceptedMethods("GET", true);
			}
			else if (method == "POST")
			{
				if (location.getAcceptedMethods()["POST"] == true)
					throw std::runtime_error("The POST method value is alraedy set as accepted method.");
				location.setAcceptedMethods("POST", true);
			}
			else if (method == "DELETE")
			{
				if (location.getAcceptedMethods()["DELETE"] == true)
					throw std::runtime_error("The DELETE method value is alraedy set as accepted method.");
				location.setAcceptedMethods("DELETE", true);
			}
			else
				throw std::runtime_error("A location accepted method value is not recognized.");
		}
	}
	if (location.getAcceptedMethods().empty())
		throw std::runtime_error("The location accepted methods directive is empty or contains only whitespaces.");
}

void	checkLocation(Locations &location, std::istringstream &iss, std::string directive)
{
	if (directive == "root")
		checkLocationRoot(location, iss);
	else if (directive == "index")
		checkLocationIndex(location, iss);
	else if (directive == "autoindex")
		checkLocationAutoIndex(location, iss);
	else if (directive == "accepted_methods")
		checkLocationAcceptedMethods(location, iss);
	else
		throw std::runtime_error("The location directive contains an unknown or unsupported directive.");
}
