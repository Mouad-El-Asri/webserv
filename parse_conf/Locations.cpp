#include "../includes/Locations.hpp"

Locations::Locations() 
{
	this->location = "";
	this->root = "";
	this->acceptedMethods["POST"] = false;
	this->acceptedMethods["GET"] = false;
	this->acceptedMethods["DELETE"] = false;
	this->index.clear();
	this->autoIndex = false;
	this->Return = "";
	this->cgi.clear();
}

Locations::Locations(const Locations& other)
{
	(*this) = other;
}

Locations&	Locations::operator=(const Locations &other)
{
	if (this != &other)
	{
		this->location = other.location;
		this->root = other.root;
		this->acceptedMethods = other.acceptedMethods;
		this->index = other.index;
		this->autoIndex = other.autoIndex;
		this->Return = other.Return;
		this->cgi = other.cgi;
	}
	return (*this);
}

void	Locations::clearLocation()
{
	this->location = "";
	this->root = "";
	this->acceptedMethods["POST"] = false;
	this->acceptedMethods["GET"] = false;
	this->acceptedMethods["DELETE"] = false;
	this->index.clear();
	this->autoIndex = false;
	this->Return = "";
	this->cgi.clear();
}

std::string	Locations::getLocation() const
{
	return (this->location);
}

std::string	Locations::getRoot() const
{
	return (this->root);
}

std::map<std::string, bool> Locations::getAcceptedMethods() const
{
	return (this->acceptedMethods);
}

std::vector<std::string> Locations::getIndex() const
{
	return (this->index);
}

bool	Locations::getAutoIndex() const
{
	return (this->autoIndex);
}

std::string	Locations::getReturn() const
{
	return (this->Return);
}

std::map<std::string, std::string>	Locations::getCgi() const
{
	return (this->cgi);
}

void	Locations::setLocation(const std::string& location)
{
	this->location = location;
}

void	Locations::setRoot(const std::string& root)
{
	this->root = root;
}

void	Locations::setAcceptedMethods(const std::string& key, const bool& value)
{
	this->acceptedMethods[key] = value;
}

void	Locations::setIndex(const std::string& index)
{
	this->index.push_back(index);
}

void	Locations::setAutoIndex(const bool& autoIndex)
{
	this->autoIndex = autoIndex;
}

void	Locations::setReturn(const std::string& Return)
{
	this->Return = Return;
}

void	Locations::setCgi(const std::string& key, const std::string& value)
{
	this->cgi[key] = value;
}
