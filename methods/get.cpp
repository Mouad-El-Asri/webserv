#include "get.hpp"
#include "../Multiplexing/multiplexing.hpp"
#define P 8081
 


// void Cgi_handling(std::string& path, info &inf)
// {
// 	struct stat st;
// 	std::string cgi;
	
// 	if (path.find(".php") != std::string::npos)
// 	{
// 		cgi = path.substr(path.find(".php") + 4, path.size());
// 		path = path.substr(0, path.find(".php") + 4);
// 		if (path.find("?") != std::string::npos)
// 		{
// 			cgi = path.substr(path.find("?") + 1, path.size());
// 			path = path.substr(0, path.find("?"));
// 		}
// 	}
// 	else if (path.find(".py") != std::string::npos)
// 	{
// 		cgi = path.substr(path.find(".py") + 3, path.size());
// 		path = path.substr(0, path.find(".py") + 3);
// 	}
// }

// int check_is_cgi(std::string& path, std::string& real_path, std::string& extansion)
// {
// 	int count = 0, i = 0, a = 0;	
// 	if (path.find(".php") != std::string::npos)
// 	{
// 		if (path.find(".php") + 4 == path.find("?"))
// 		{
// 			extansion = ".php";
// 			path = path.substr(0, path.find("?"));
// 			return 1;
// 		}
// 		else if (path.find_last_of(".php") +4  == std::string::npos)
// 		{
// 			extansion = ".php";
// 			path = path.substr(0, path.find_last_of(".php") + 4);
// 			return 1;
// 		}
// 	}
// 	else if (path.find(".py") != std::string::npos)
// 	{
// 		if (path.find(".py") + 3 == path.find("?"))
// 		{
// 			extansion = ".py";
// 			path = path.substr(0, path.find("?"));
// 			return 1;
// 		}
// 		else if (path.find_last_of(".py") + 3  == std::string::npos)
// 		{
// 			extansion = ".py";
// 			path = path.substr(0, path.find_last_of(".py") + 3);
// 			return 1;
// 		}
// 	}
// 	return 0;
// }

void method_get::check_location()
{
	int forbidden = 0;
	std::string let;
	size_t i = 0;
	int max_lenght = 0;
	// int checked_cgi = 0;
	int size = 0;
	int is_checked = 0;
	while (i < keep.getLocationsVec().size())
	{
		size = keep.getLocationsVec()[i].getLocation().size();
		if (keep.getLocationsVec()[i].getLocation() == url.substr(0, size) && size >= max_lenght)
		{
			if (keep.getLocationsVec()[i].getAcceptedMethods()["GET"] == true)
			{
				max_lenght = size;
				this->auto_index = keep.getLocationsVec()[i].getAutoIndex();
				this->index = keep.getLocationsVec()[i].getIndex();
				this->route = keep.getLocationsVec()[i].getRoot();
				if (route[route.size() - 1] != '/')
					route += "/";
				path = route + url.substr(size, url.size());
				// if (!keep.getLocationsVec()[i].getCgi().empty())
				// {
				// 	std::string ext;
				// 	if (check_is_cgi(path, let, ext)== 1)
				// 	{
				// 		if (keep.getLocationsVec()[i].getCgi()[ext] == ".php" || keep.getLocationsVec()[i].getCgi()[ext] == ".py")
				// 		{
				// 			checked_cgi = 1;
				// 		}
				// 		else 
				// 			checked_cgi = 0;
				// 	}
				// }
				// else 
				// 	checked_cgi = 0;
				// if (keep.getLocationsVec()[i].getReturn() != "")
				// {
				// 	this->infa.buffer_to_send = "HTTP/1.1 " + keep.getLocationsVec()[i].getReturn() + "\nContent-Type: text/html\nContent-Length: 0\n\n";
				// 	this->infa.status = 1;
				// 	throw std::exception();					
				// }
				is_checked = 1;
				forbidden = 0;
			}
			else
				forbidden = 1;
		}
		i++;
	}

	if (forbidden == 1)
	{
		set_error_403();
		throw std::exception();
	}
	// if (checked_cgi == 1)
	// 	execute_cgi(let);
	if (is_checked == 0)
	{
		set_error_404();
		throw std::exception();
	}
}

void method_get::get_check_path()
{
	struct stat st;
	try {
		std::cout << "path: " << path << std::endl;
	check_location();
	}
	catch (std::exception &e)
	{
		return ;
	}
	if (stat(path.c_str() , &st) == 0)
	{
		try{
	    if (access(path.c_str(), W_OK | X_OK) == -1)
        {
            set_error_403();
            throw std::exception();
        }
			this->size = st.st_size;
			infa.size = st.st_size;
			if (S_ISDIR(st.st_mode))
				folder_handling();
			else
				file_handling();
		}
		catch (const std::exception& e)
		{
			return ;
		}
	}
	else 
	{
		set_error_404();
	}
}


method_get::method_get(Directives& k , std::string url, info &inf) : infa(inf) , keep(k)
{
	this->keep = k;
	std::string route = k.getRoot();
	this->path = route + url;
	auto_index = k.getAutoIndex();
	this->url = url;
	infa.path = path;
	this->erros_page = k.getErrorPages();
	std::string moth = "HTTP/1.1 200 OK\n";
	extansion_handling[".html"] = moth + "Content-Type: text/html\nContent-Length: ";
	extansion_handling[".css"] = moth + "Content-Type: text/css\nContent-Length: ";
	extansion_handling[".js"] = moth + "Content-Type: text/javascript\nContent-Length: ";
	extansion_handling[".jpg"] = moth + "Content-Type: image/jpeg\nContent-Length: ";
	extansion_handling[".jpeg"] = moth + "Content-Type: image/jpeg\nContent-Length: ";
	extansion_handling[".png"] = moth + "Content-Type: image/png\nContent-Length: ";
	extansion_handling[".gif"] = moth + "Content-Type: image/gif\nContent-Length: ";
	extansion_handling[".mp4"] = moth + "Content-Type: video/mp4\nContent-Length: ";
	extansion_handling[".mp3"] = moth + "Content-Type: audio/mpeg\nContent-Length: ";
	extansion_handling[".pdf"] = moth + "Content-Type: application/pdf\nContent-Length: ";
	extansion_handling[".txt"] = moth + "Content-Type: text/plain\nContent-Length: ";
	extansion_handling["404"] = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ";
	extansion_handling["403"] = "HTTP/1.1 403 Forbidden\nContent-Type: text/html\nContent-Length: ";
	extansion_handling["500"] = "HTTP/1.1 500 Internal Server Error\nContent-Type: text/html\nContent-Length: ";
}


void ft_get(Directives &data, std::string url,  info &socket)
{
	if (socket.was_read == 1)
		return ;
	method_get get(data, url,  socket);
	get.get_check_path();
	socket.was_read = 1;
}

void close_socket(std::vector<info> &clientes, size_t& i , fd_set &writefds, fd_set &readfds)
{
	if (clientes[i].file)
		clientes[i].file->close();
	FD_CLR(clientes[i].socket, &writefds);
	FD_CLR(clientes[i].socket, &readfds);
	close(clientes[i].socket);
	delete clientes[i].file;
	clientes.erase(clientes.begin() + i);
}

void	get_response(info &clientes, t_client_info *client_write , t_client_info **clients, fd_set &reads, fd_set &writes)
{
	char bu[1025];
	static int	count = 0;
	if (clientes.status == 1)
	{
		std::cout << "--------------> " << clientes.buffer_to_send <<  std::endl;
		if (send(clientes.socket, clientes.buffer_to_send.c_str(), clientes.buffer_to_send.size(), 0) < 0)
		{
			if (clientes.file)
				clientes.file->close();
			drop_client(client_write, clients, reads, writes);
			return ;
		}
		clientes.status = 0;
	}
	if (clientes.file && clientes.file->eof() == false)
	{
		clientes.file->read(bu, 1024);
		int reading = clientes.file->gcount();
		int sending   = send(clientes.socket, bu, reading, 0) ;
		count += sending;
		if	(sending < 0)
		{
			std::cout << "FAILED TO SEND THE File" << std::endl;
			if (clientes.file)
				clientes.file->close();
			drop_client(client_write, clients, reads, writes);
			return ;
		}
		if(sending == 0)
		{
			std::cout << "hunging conection---------------------------------- " << std::endl;
		}
	}
	else
	{
		std::cout << "ending conection " << std::endl;
		if (clientes.file)
			clientes.file->close();
		std::cout << "the original size : " << clientes.size  << "      sended size     " << count << std::endl;
		count = 0;
		drop_client(client_write, clients, reads, writes);
		return ;
	}
}
