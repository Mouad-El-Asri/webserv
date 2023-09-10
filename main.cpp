#include "includes/conf_parser.hpp"

int	main(int argc, char **argv)
{
	try
	{
		std::ifstream conf = checkArgs(argc, argv);
		readAndCheckConf(conf);
	}
	catch(const std::exception &e)
	{
		std::cerr << "Error : " << e.what() << "\n";
        return (1);
	}
	return (0);
}

