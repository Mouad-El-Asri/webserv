#ifndef PARSE_CONF_HPP
#define PARSE_CONF_HPP

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>

std::ifstream	checkArgs(int argc, char **argv);
void			skipEmptyLinesAndCheckServerBlock(std::ifstream &conf);
void			readAndCheckConf(std::ifstream &conf);

#endif