#include "../includes/Directives.hpp"

Directives::Directives() 
{
	this->serverNames.clear();
	this->listen = {};
	this->errorPages[400] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/400_bad_request.html";
	this->errorPages[401] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/401_unauthorized.html";
	this->errorPages[403] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/403_forbidden.html";
	this->errorPages[404] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/404_not_found.html";
	this->errorPages[500] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/500_internal_service_error.html";
	this->errorPages[503] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/503_service_unavailable.html";
	this->maxBodySizeInBytes = "";
	this->index = "";
	this->root = "";
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
		this->index = other.index;
		this->root = other.root;
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

std::string	Directives::getIndex() const
{
	return (this->index);
}

std::string	Directives::getRoot() const
{
	return (this->root);
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

void	Directives::setIndex(const std::string& index)
{
	this->index = index;
}

void	Directives::setRoot(const std::string& root)
{
	this->root = root;
}
