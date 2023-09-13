#ifndef PARSE_CONF_HPP
#define PARSE_CONF_HPP

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include "utils.hpp"

std::ifstream	checkArgs(int argc, char **argv);
void			skipEmptyLinesAndCheckServerBlock(std::ifstream &conf, bool flag);
void			checkServerName(Directives &directives, std::istringstream &iss);
void			checkListen(Directives &directives, std::istringstream &iss);
void			checkRoot(Directives &directives, std::istringstream &iss);
void			checkIndex(Directives &directives, std::istringstream &iss);
void			addServerDirectivesToServers(Directives &serverDirectives, Servers &servers);
void			readAndCheckConf(std::ifstream &conf);

#endif