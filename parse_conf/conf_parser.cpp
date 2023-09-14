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
	if (access(path.c_str(), F_OK) != 0)
		throw std::runtime_error("The root path can't be opened due to an error.");
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
	std::cout << size << std::endl;
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
	Servers 	servers;
	int			endBracketNum;
	int			startBracketNum;
	bool		serverBlockIsNotEmpty;

	serverBlockIsNotEmpty = false;
	startBracketNum = 1;
	endBracketNum = 0;
	while (std::getline(conf, line))
	{
		if (line.empty() || isOnlyWhitespaces(line))
			continue ;
		if (charIsInString(line, '{'))
		{
			startBracketNum++;
			continue ;
		}
		if (trimSpaces(line) == "}")
		{
			if (!serverBlockIsNotEmpty)
				throw std::runtime_error("Server block is empty.");
			endBracketNum++;
			if (startBracketNum == endBracketNum)
			{
				addServerDirectivesToServers(serverDirectives, servers);
				skipEmptyLinesAndCheckServerBlock(conf, false);
			}
			continue ;
		}
		serverBlockIsNotEmpty = true;
		line = removeExtraWhitespace(trimSpaces(line));
		std::istringstream iss(line);
		std::string directive;

		std::getline(iss, directive, ' ');
		directive = trimSpaces(directive);
		if (directive == "server_name")
			checkServerName(serverDirectives, iss);
		else if (directive == "listen")
			checkListen(serverDirectives, iss);
		else if (directive == "root")
			checkRoot(serverDirectives, iss);
		else if (directive == "index")
			checkIndex(serverDirectives, iss);
		else if (directive == "client_max_body_size")
			checkMaxBodySize(serverDirectives, iss);
	}
	if (startBracketNum != endBracketNum)
		throw std::runtime_error("The config file syntax is incorrect, there is no ending bracket.");
}
