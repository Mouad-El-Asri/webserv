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
		std::map<int, std::string> errorPages;
		std::string maxBodySizeInBytes;

	public:
		Directives();

		std::string getServerName() const;
    	std::map<std::string, int> getListen() const;
    	std::map<int, std::string> getErrorPages() const;
		std::string	getMaxBodySizeInBytes() const;
		
    	void setServerName(const std::string& serverName);
   		void setListen(const std::map<std::string, int>& listen);
    	void setErrorPages(const std::map<int, std::string>& errorPages);
    	void setMaxBodySizeInBytes(const std::string& maxBodySizeInBytes);
};

#endif