#include "parse_conf/conf_parser.hpp"

int	main(int argc, char **argv)
{
	try
	{
		checkArgs(argc, argv);
	}
	catch(const std::exception &e)
	{
		std::cerr << "Error : " << e.what() << "\n";
        return (1);
	}
	return (0);
}
