#include "../includes/conf_parser.hpp"

void	checkArgs(int argc, char **argv, std::ifstream &conf)
{
	if (argc != 2)
		throw std::invalid_argument("Invalid number of arguments.");
	else if (std::string(argv[1]).rfind(".conf") != (std::string(argv[1]).length() - 5))
		throw std::invalid_argument("Invalid config file extension.");

	if (!conf.is_open())
		throw std::runtime_error("Failed to open conf file : " + std::string(argv[1]) + ".");
}

void	checkLocation(Directives &directives, Locations &location, std::istringstream &iss, std::string directive)
{
	if (directive == "root")
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
	else if (directive == "index")
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
	else if (directive == "autoindex")
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
	else if (directive == "accepted_methods")
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
	else
		throw std::runtime_error("A location directive can't be recognized.");
	directives.setLocation(location);
}

void	addServerDirectivesToServers(Directives &serverDirectives, Servers &servers)
{
	Directives	serverblock = serverDirectives;
	servers.setServer(serverblock);
}

void	skipEmptyLinesAndCheckServerBlock(std::ifstream &conf, bool flag)
{
	std::string	line;
	bool fileIsEmpty;
	
	fileIsEmpty = flag;
	while (std::getline(conf, line))
	{
		if (line.empty() || isOnlyWhitespaces(line))
			continue ;
		removeWhitespaces(line);
		if (line == "server{")
		{
			fileIsEmpty = false;
			readAndCheckConf(conf);
		}
		else
			throw std::runtime_error("Invalid format in the config file.");
	}
	if (fileIsEmpty)
		throw std::runtime_error("The config file is empty.");
}

void	readAndCheckConf(std::ifstream &conf)
{
	std::string	line;
	Directives	serverDirectives;
	Locations	serverLocations;
	Servers 	servers;
	int			endBracketNum;
	int			startBracketNum;
	bool		serverBlockIsNotEmpty;
	bool		isLocationBlck;
	bool		locationBlockIsNotEmpty;

	startBracketNum = 1;
	endBracketNum = 0;
	serverBlockIsNotEmpty = false;
	isLocationBlck = false;
	locationBlockIsNotEmpty = false;
	while (std::getline(conf, line))
	{
		if (line.empty() || isOnlyWhitespaces(line))
			continue ;
		if (isLocationBlock(trimSpaces(line)))
		{
			isLocationBlck = true;
			startBracketNum++;
		}
		if (trimSpaces(line) == "}")
		{
			if (!serverBlockIsNotEmpty)
				throw std::runtime_error("Server block is empty.");
			if (isLocationBlck && !locationBlockIsNotEmpty)
				throw std::runtime_error("location block is empty.");
			isLocationBlck = false;
			locationBlockIsNotEmpty = false;
			endBracketNum++;
			if (startBracketNum == endBracketNum)
			{
				addServerDirectivesToServers(serverDirectives, servers);
				skipEmptyLinesAndCheckServerBlock(conf, false);
			}
			continue ;
		}
		serverBlockIsNotEmpty = true;
		line = trimSpaces(line);

		std::istringstream iss(line);
		std::string directive;

		std::getline(iss, directive, ' ');
		directive = trimSpaces(directive);

		if (isLocationBlck)
		{
			if (directive == "location")
			{
				std::string locationDirective;
				std::getline(iss, locationDirective);
				if (locationDirective.empty() || isOnlyWhitespaces(locationDirective) || locationDirective == "{")
					throw std::runtime_error("The location directive is empty or contains only whitespaces.");
				locationDirective.resize(locationDirective.length() - 1);
				locationDirective = trimSpaces(locationDirective);
				if (containsWhitespace(locationDirective))
					throw std::runtime_error("The location directive contains whitespaces.");
				serverLocations.setLocation(locationDirective);
				continue ;
			}
			locationBlockIsNotEmpty = true;
			checkLocation(serverDirectives, serverLocations, iss, directive);
		}
		else if (directive == "server_name")
			checkServerName(serverDirectives, iss);
		else if (directive == "listen")
			checkListen(serverDirectives, iss);
		else if (directive == "root")
			checkRoot(serverDirectives, iss);
		else if (directive == "index")
			checkIndex(serverDirectives, iss);
		else if (directive == "client_max_body_size")
			checkMaxBodySize(serverDirectives, iss);
		else if (directive == "error_page")
			checkErrorPages(serverDirectives, iss);
		else if (directive == "autoindex")
			checkAutoIndex(serverDirectives, iss);
	}
	if (startBracketNum != endBracketNum)
		throw std::runtime_error("The config file syntax is incorrect, there is no ending bracket.");
}
