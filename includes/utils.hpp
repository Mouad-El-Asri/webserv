#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

bool		isOnlyWhitespaces(const std::string& str);
std::string	trimSpaces(const std::string& str);
void		removeWhitespaces(std::string& str);
bool		charIsInString(const std::string& str, const char ch);

#endif