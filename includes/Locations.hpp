#ifndef LOCATIONS_HPP
#define LOCATIONS_HPP

#include <vector>
#include <iostream>

class Locations
{
	private:
		std::string					location;
		std::string					root;
		std::vector<std::string>	acceptedMethods;
		std::vector<std::string>	index;
		bool						autoIndex;

	public:
		Locations();

		std::string					getLocation() const;
		std::string					getRoot() const;
		std::vector<std::string>	getAcceptedMethods() const;
		std::vector<std::string>	getIndex() const;
		bool						getAutoIndex() const;

		void	setLocation(const std::string& location);
    	void	setRoot(const std::string& root);
    	void	setAcceptedMethods(const std::string& index);
    	void	setIndex(const std::string& index);
		void	setAutoIndex(const bool& autoIndex);
};

#endif