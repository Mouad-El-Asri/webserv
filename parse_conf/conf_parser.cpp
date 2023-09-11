#include "../includes/conf_parser.hpp"
#include "../includes/Directives.hpp"
#include "../includes/utils.hpp"

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

void	skipEmptyLinesAndCheckServerBlock(std::ifstream &conf, bool flag)
{
	std::string	line;
	bool fileIsEmpty;
	
	fileIsEmpty = true;
	if (flag)
		return ;
	while (std::getline(conf, line))
	{
		if (line.empty() || isOnlyWhitespaces(line))
			continue ;
		removeWhitespaces(line);
		if (line == "server{")
		{
			fileIsEmpty = false;
			break ;
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
	Directives	directives;
	int			endBracketNum;
	int			startBracketNum;
	bool		serverBlockIsNotEmpty;

	skipEmptyLinesAndCheckServerBlock(conf, false);
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
			removeWhitespaces(line);
			if (line == "server{")
			{
				skipEmptyLinesAndCheckServerBlock(conf, true);
				serverBlockIsNotEmpty = false;
			}
			continue ;
		}
		if (trimSpaces(line) == "}")
		{
			if (!serverBlockIsNotEmpty)
				throw std::runtime_error("Server block is empty.");
			endBracketNum++;
			continue ;
		}
		serverBlockIsNotEmpty = true;
		line = trimSpaces(line);
		std::istringstream iss(line);
		std::string directive;

		std::getline(iss, directive, ' ');
		directive = trimSpaces(directive);
		if (directive == "server_name")
			checkServerName(directives, iss);
	}
	if (startBracketNum != endBracketNum)
		throw std::runtime_error("The config file syntax is incorrect, there is no ending bracket.");
}

void	checkServerName(Directives&	directives, std::istringstream& iss)
{
	std::string value;

	while (std::getline(iss, value, ' '))
	{
		value = trimSpaces(value);
		if (isAlphanumeric(value) || isIPAddress(value) || isDomainName(value))
			directives.setServerName(value);
	}
	if (directives.getServerNames().empty())
		throw std::runtime_error("The server_name directive is not valid.");
}
