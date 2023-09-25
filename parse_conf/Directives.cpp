#include "../includes/Directives.hpp"

Directives::Directives() 
{
	this->serverName = "";
	this->host = "";
	this->listen = -1;
	this->errorPages[400] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/400_bad_request.html";
	this->errorPages[401] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/401_unauthorized.html";
	this->errorPages[403] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/403_forbidden.html";
	this->errorPages[404] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/404_not_found.html";
	this->errorPages[500] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/500_internal_service_error.html";
	this->errorPages[503] = "/nfs/homes/moel-asr/Desktop/webserv/default_status_code_pages/503_service_unavailable.html";
	this->maxBodySizeInBytes = -1;
	this->index.clear();
	this->root = "";
	this->autoIndex = false;
	this->locationsVec.clear();
}

Directives::Directives(const Directives& other)
{
	(*this) = other;
}

Directives&	Directives::operator=(const Directives &other)
{
	if (this != &other)
	{
		this->serverName = other.serverName;
		this->host = other.host;
		this->listen = other.listen;
		this->errorPages = other.errorPages;
		this->maxBodySizeInBytes = other.maxBodySizeInBytes;
		this->index = other.index;
		this->root = other.root;
		this->autoIndex = other.autoIndex;
		this->locationsVec = other.locationsVec;
	}
	return (*this);
}

std::string	Directives::getServerName() const
{
	return (this->serverName);
}

std::string	Directives::getHost() const
{
	return (this->host);
}

int	Directives::getListen() const
{
	return (this->listen);
}

std::map<int, std::string>	Directives::getErrorPages() const
{
	return (this->errorPages);
}

int	Directives::getMaxBodySizeInBytes() const
{
	return (this->maxBodySizeInBytes);
}

std::vector<std::string> Directives::getIndex() const
{
	return (this->index);
}

std::string	Directives::getRoot() const
{
	return (this->root);
}

bool	Directives::getAutoIndex() const
{
	return (this->autoIndex);
}

std::vector<Locations>	Directives::getLocationsVec() const
{
	return (this->locationsVec);
}

void	Directives::setServerName(const std::string& serverName)
{
	this->serverName = serverName;
}

void	Directives::setHost(const std::string& host)
{
	this->host = host;
}

void	Directives::setListen(const int& port)
{
    this->listen = port;
}

void	Directives::setErrorPage(const int& errorCode, const std::string& page)
{
    this->errorPages[errorCode] = page;
}

void	Directives::setMaxBodySizeInBytes(const int& maxBodySizeInBytes)
{
	this->maxBodySizeInBytes = maxBodySizeInBytes;
}

void	Directives::setIndex(const std::string& index)
{
	this->index.push_back(index);
}

void	Directives::setRoot(const std::string& root)
{
	this->root = root;
}

void	Directives::setAutoIndex(const bool& autoIndex)
{
	this->autoIndex = autoIndex;
}

void	Directives::setLocation(const Locations& location)
{
	this->locationsVec.push_back(location);
}
