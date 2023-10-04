#include "get.hpp"

void method_get::set_error_500()
{
	std::cout << "error 500" << std::endl;
	std::ofstream file("./error500.html");
	file << "<html><body><h1>500 Internal Server Error</h1></body></html>";
	file.close();
	this ->path  = "./error500.html";
	struct stat st;
	stat(path.c_str() , &st);
	std::stringstream ss ;
	ss << st.st_size;
	infa.buffer_to_send = extansion_handling["500"] + ss.str() + "\r\n\r\n";
	infa.file = new std::ifstream(path.c_str(),  std::ios::binary);
}

void	method_get::set_error_403()
{
	std::cout << "error 403" << std::endl;	
	struct stat st;

	this ->path  = erros_page["403"];
	this->file = new std::ifstream(path.c_str(),  std::ios::binary);
	if (!file->is_open())
	{
		std::cout << "error 500" << std::endl;
		infa.buffer_to_send = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n <html><body><h1>403 Forbidden</h1></body></html>";
		infa.status = 1;
		infa.file = NULL;
		return;
	}
	stat(path.c_str() , &st);

	this->size = st.st_size;
	infa.size = st.st_size;
	std::stringstream ss ;
	std::string ext;
	ext = extansion_handling["403"];
	ss << this->size;
	ext += ss.str();
	ext += "\r\n";
	ext += "\r\n";
	this->infa.buffer_to_send = ext;
	this->infa.status = 1;
	infa.file = file;
}

void	method_get::set_error_404()
{
	std::cout << "error 404" << std::endl;
	struct stat st;
	this ->path  = erros_page["404"];
	infa.file = new std::ifstream(path.c_str(),  std::ios::binary);
	if (!infa.file->is_open())
	{
		infa.buffer_to_send = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 20\r\n\r\n <html><body><h1>404 Not Found</h1></body></html>";
		infa.status = 1;
		infa.file = NULL;
		return;
	}
	stat(path.c_str() , &st);
	this->size = 		st.st_size;
	infa.size = 		st.st_size;
	std::stringstream	ss;
	std::string			ext;
	ext = extansion_handling["404"];
	ss << this->size;
	ext += ss.str();
	ext += "\r\n";
	ext += "\r\n";
	this->infa.buffer_to_send = ext;
	this->infa.status = 1;
}
