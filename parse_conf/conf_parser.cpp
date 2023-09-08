#include "conf_parser.hpp"

std::ifstream	checkArgs(int argc, char **argv)
{
	if (argc != 2)
		throw std::invalid_argument("Invalid number of arguments.");
	else if (std::string(argv[1]).rfind(".conf") != (std::string(argv[1]).length() - 5))
		throw std::invalid_argument("Invalid config file extension.");

	std::string confFile = argv[1];
	std::ifstream file(confFile);

	if (!file.is_open())
		throw std::runtime_error("Failed to open conf file : " + std::string(confFile) + ".");
	return (file);
}
