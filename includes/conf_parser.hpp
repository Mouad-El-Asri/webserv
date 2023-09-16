#ifndef PARSE_CONF_HPP
#define PARSE_CONF_HPP

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "check_directives.hpp"

void			checkArgs(int argc, char **argv, std::ifstream &conf);
void	        skipEmptyLinesAndCheckServerBlock(std::ifstream &conf, bool flag, Servers &servers);
void			addServerDirectivesToServers(Directives &serverDirectives, Servers &servers);
void	        readAndCheckConf(std::ifstream &conf, Servers &servers);
void			checkLocation(Locations &location, std::istringstream &iss, std::string directive);

#endif