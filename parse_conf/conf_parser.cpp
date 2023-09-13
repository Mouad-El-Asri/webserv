#include "../includes/conf_parser.hpp"

std::ifstream	checkArgs(int argc, char **argv)
{
	if (argc != 2)
		throw std::invalid_argument("Invalid number of arguments.");
	else if (std::string(argv[1]).rfind(".conf") != (std::string(argv[1]).length() - 5))
		throw std::invalid_argument("Invalid config file extension.");

	char *confFile = argv[1];
	std::ifstream file(confFile);

	if (!file.is_open())
		throw std::runtime_error("Failed to open conf file : " + std::string(confFile) + ".");
	return (file);
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
		throw std::runtime_error("The server_name directive value is empty.");
}

void	checkListen(Directives &directives, std::istringstream &iss)
{
	std::string value;

	std::getline(iss, value);
	if (value.empty() || isOnlyWhitespaces(value))
		throw std::runtime_error("The listen directive valuye is empty or contains only whitespaces.");
	value = trimSpaces(value);
	if (containsWhitespace(value))
		throw std::runtime_error("The listen value contains whitespaces.");
	(void)directives;
	std::vector<std::string> result;
	splitString(value, result);
	if (!isNum(result[1]))
		throw std::runtime_error("The listen port is not a valid number.");
	if (!(isAlphanumeric(result[0])) && !(isIPAddress(result[0])) && !(isDomainName(result[0])))
		throw std::runtime_error("The listen directive host is not valid.");
	directives.setListen(result[0], atoi(result[1].c_str()));
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
		line = trimSpaces(line);
		std::istringstream iss(line);
		std::string directive;

		std::getline(iss, directive, ' ');
		directive = trimSpaces(directive);
		if (directive == "server_name")
			checkServerName(serverDirectives, iss);
		else if (directive == "listen")
			checkListen(serverDirectives, iss);
		// else if (directive == "error_page")
		// 	//;
	}
	if (startBracketNum != endBracketNum)
		throw std::runtime_error("The config file syntax is incorrect, there is no ending bracket.");
}
