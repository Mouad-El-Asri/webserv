#include "../includes/Directives.hpp"

Directives::Directives() 
{
	this->serverName = "";
	this->listen = {};
	this->errorPages = {};
	this->maxBodySizeInBytes = "";
}

std::string	Directives::getServerName() const
{
	return (this->serverName);
}

std::map<std::string, int>	Directives::getListen() const
{
	return (this->listen);
}

std::map<int, std::string>	Directives::getErrorPages() const
{
	return (this->errorPages);
}

std::string	Directives::getMaxBodySizeInBytes() const
{
	return (this->maxBodySizeInBytes);
}

		
void	Directives::setServerName(const std::string& serverName)
{
	this->serverName = serverName;
}

void	Directives::setListen(const std::string& key, const int& value)
{
    this->listen[key] = value;
}

void	Directives::setErrorPage(const int& errorCode, const std::string& page)
{
    this->errorPages[errorCode] = page;
}


void	Directives::setMaxBodySizeInBytes(const std::string& maxBodySizeInBytes)
{
	this->maxBodySizeInBytes = maxBodySizeInBytes;
}
