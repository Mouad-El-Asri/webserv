#include "Locations.hpp"

Locations::Locations() 
{
	this->root = "";
	this->acceptedMethods.clear();
	this->index.clear();
	this->autoIndex = false;
}

std::string	Locations::getLocation() const
{
	return (this->location);
}

std::string	Locations::getRoot() const
{
	return (this->root);
}

std::vector<std::string> Locations::getAcceptedMethods() const
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

void	Locations::setLocation(const std::string& location)
{
	this->location = location;
}

void	Locations::setRoot(const std::string& root)
{
	this->root = root;
}

void	Locations::setAcceptedMethods(const std::string& acceptedMethods)
{
	this->acceptedMethods.push_back(acceptedMethods);
}

void	Locations::setIndex(const std::string& index)
{
	this->index.push_back(index);
}

void	Locations::setAutoIndex(const bool& autoIndex)
{
	this->autoIndex = autoIndex;
}
