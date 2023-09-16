#include "includes/conf_parser.hpp"

int	main(int argc, char **argv)
{
	try
	{
		Servers 	servers;
		if (argc != 2)
			throw std::invalid_argument("Invalid number of arguments.");
		std::ifstream conf(argv[1]);
		checkArgs(argv, conf);
		skipEmptyLinesAndCheckServerBlock(conf, true, servers);
	}
	catch(const std::exception &e)
	{
		std::cerr << "Error : " << e.what() << "\n";
        return (1);
	}
	return (0);
}
