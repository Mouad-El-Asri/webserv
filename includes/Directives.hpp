#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

class Directives
{
	private:
		std::vector<std::string>	serverNames;
		std::map<std::string, int>	listen;
		std::map<int, std::string>	errorPages;
		std::string					maxBodySizeInBytes;
		std::string					index;
		std::string					root;

	public:
		Directives();
		Directives(const Directives& other);
		Directives&			operator=(const Directives& other);

		std::vector<std::string>	getServerNames() const;
    	std::map<std::string, int>	getListen() const;
    	std::map<int, std::string>	getErrorPages() const;
		std::string					getMaxBodySizeInBytes() const;
		std::string					getIndex() const;
		std::string					getRoot() const;
		
    	void setServerName(const std::string& serverName);
   		void setListen(const std::string& key, const int& value);
    	void setErrorPage(const int& errorCode, const std::string& page);
    	void setMaxBodySizeInBytes(const std::string& maxBodySizeInBytes);
    	void setIndex(const std::string& index);
    	void setRoot(const std::string& root);
};

#endif