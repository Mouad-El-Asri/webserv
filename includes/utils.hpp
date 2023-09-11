#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>

bool		isOnlyWhitespaces(const std::string& str);
std::string	trimSpaces(const std::string& str);
void		removeWhitespaces(std::string& str);
bool		charIsInString(const std::string& str, const char ch);
bool		isIPAddress(const std::string &str);
bool		isValidDomainSegment(const std::string &segment);
bool		isDomainName(const std::string &str);
bool		isAlphanumeric(const std::string &str);

#endif