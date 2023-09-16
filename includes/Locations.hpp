#ifndef LOCATIONS_HPP
#define LOCATIONS_HPP

#include <vector>
#include <map>
#include <iostream>

class Locations
{
	private:
		std::string					location;
		std::string					root;
		std::map<std::string, bool>	acceptedMethods;
		std::vector<std::string>	index;
		bool						autoIndex;

	public:
		Locations();
		Locations(const Locations& other);
		Locations&			operator=(const Locations& other);

		void	clearLocation();

		std::string					getLocation() const;
		std::string					getRoot() const;
		std::map<std::string, bool>	getAcceptedMethods() const;
		std::vector<std::string>	getIndex() const;
		bool						getAutoIndex() const;

		void	setLocation(const std::string& location);
    	void	setRoot(const std::string& root);
    	void	setAcceptedMethods(const std::string& key, const bool& value);
    	void	setIndex(const std::string& index);
		void	setAutoIndex(const bool& autoIndex);
};

#endif