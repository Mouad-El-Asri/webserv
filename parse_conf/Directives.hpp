#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP

#include <iostream>
#include <map>
#include <string>

class Directives
{
	private:
		std::string serverName;
		std::map<std::string, int> listen;
		std::map<int, std::string> error_pages;

	public:
		
};

#endif