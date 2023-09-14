#ifndef PARSE_CONF_HPP
#define PARSE_CONF_HPP

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include "utils.hpp"

void			checkArgs(int argc, char **argv, std::ifstream &conf);
void			skipEmptyLinesAndCheckServerBlock(std::ifstream &conf, bool flag);
void			checkServerName(Directives &directives, std::istringstream &iss);
void			checkListen(Directives &directives, std::istringstream &iss);
void			checkRoot(Directives &directives, std::istringstream &iss);
void			checkIndex(Directives &directives, std::istringstream &iss);
void			checkMaxBodySize(Directives &directives, std::istringstream &iss);
void			checkErrorPages(Directives &directives, std::istringstream &iss);
void			checkAutoIndex(Directives &directives, std::istringstream &iss);
void			addServerDirectivesToServers(Directives &serverDirectives, Servers &servers);
void			readAndCheckConf(std::ifstream &conf);

#endif