#include "get.hpp"

void method_get::file_handling()
{
	std::cout << "path : " << this->path << std::endl;
	infa.file = new std::ifstream(path.c_str(), std::ios::binary);
	std::stringstream ss ;
	std::string ext;
	size_t i = path.find_last_of(".");
	if (i == std::string::npos)
		ext = ".html";
	ext =  path.substr(i, path.size());
	std::string extansion =  extansion_handling[ext];
	if (extansion == "")
		extansion = extansion_handling[".html"];
	ss << this->size;
	extansion += ss.str();
	extansion += "\r\n";
	extansion += "\r\n";
	this->infa.buffer_to_send = extansion;
	this->infa.status = 1;
}
