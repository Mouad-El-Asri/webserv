#include "../includes/Directives.hpp"

Directives::Directives() 
{
	this->serverNames.clear();
	this->listen = {};
	this->errorPages = {};
	this->maxBodySizeInBytes = "";
}

Directives::Directives(const Directives& other)
{
	(*this) = other;
}

Directives&	Directives::operator=(const Directives &other)
{
	if (this != &other)
	{
    	this->serverNames = other.serverNames;
    	this->listen = other.listen;
    	this->errorPages = other.errorPages;
    	this->maxBodySizeInBytes = other.maxBodySizeInBytes;
	}
	return (*this);
}

std::vector<std::string>	Directives::getServerNames() const
{
	return (this->serverNames);
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
	this->serverNames.push_back(serverName);
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
