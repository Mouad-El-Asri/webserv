#ifndef PARSE_CONF_HPP
#define PARSE_CONF_HPP

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include "utils.hpp"

std::ifstream	checkArgs(int argc, char **argv);
void			skipEmptyLinesAndCheckServerBlock(std::ifstream &conf, bool flag);
void			checkServerName(Directives &directives, std::istringstream &iss);
void			readAndCheckConf(std::ifstream &conf);

#endif