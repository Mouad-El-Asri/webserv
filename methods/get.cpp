#include "get.hpp"
#include "../Multiplexing/multiplexing.hpp"
#define P 8081
 

void method_get::execute_cgi(std::string &path, std::string &extansion, std::string &arguments)
{
	int fd[2];
	pid_t pid;
	int status;
	char *argv[3];
	char *envp[2];
	std::string run_it;
	char buf[1024];
	std::string str;


	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		argv[0] = strdup(path.c_str());
		argv[1] = NULL;
		envp[0] = strdup("REQUEST_METHOD=GET");
		if (arguments != "")
			envp[1] = strdup(arguments.c_str());
		else
			envp[1] = NULL;
		if (extansion == ".php")
			run_it = "php-cgi";
		else if (extansion == ".py")
			run_it = "python3";
		execve(run_it.c_str(), argv, envp);
		exit(0);
	}
	else
	{
		close(fd[1]);
		waitpid(pid, &status, 0);
		std::ofstream temp("file.txt");

		while (read(fd[0], buf, 1024) > 0)
		{
			str = buf;
			temp << buf;
		}
		close(fd[0]);
		temp.close();
		infa.file = new std::ifstream("file.txt");
		if (infa.file->is_open() == false)
		{
			set_error_500();
			throw std::exception();
		}
		struct stat st;
		stat("file.txt", &st);
		infa.size = st.st_size;
		infa.status = 1;
		std::stringstream ss;
		ss << infa.size;
		path = "file.txt";
		file_handling();
	}
}

int check_is_cgi(std::string& path, std::string& real_path, std::string& extansion, std::string arguments)
{	

	if (path.find(".php") != std::string::npos)
	{
		if (path.find(".php") + 4 == path.find("?"))
		{
			extansion = ".php";
			real_path = path.substr(0, path.find("?"));
			arguments = "QUERY_STRING=" + path.substr(path.find("?") + 1, path.size());
			return 1;
		}
		else if (path.find_last_of(".php") +4  == std::string::npos)
		{
			extansion = ".php";
			real_path = path.substr(0, path.find_last_of(".php") + 4);
			arguments = "";
			return 1;
		}
	}
	else if (path.find(".py") != std::string::npos)
	{
		if (path.find(".py") + 3 == path.find("?"))
		{
			extansion = ".py";
			real_path = path.substr(0, path.find("?"));
			arguments = "QUERY_STRING=" + path.substr(path.find("?") + 1, path.size());
			return 1;
		}
		else if (path.find_last_of(".py") + 3  == std::string::npos)
		{
			extansion = ".py";
			real_path = path.substr(0, path.find_last_of(".py") + 3);
			arguments = "";
			return 1;
		}
	}
	return 0;
}

void method_get::check_location()
{
	int forbidden = 0;
	std::string let;
	size_t i = 0;
	int max_lenght = 0;
	std::string ext;
	std::string arguments;
	int checked_cgi = 0;
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
				std::cout << keep.getLocationsVec()[i].getCgi()[".php"] << std::endl;
				if (keep.getLocationsVec()[i].getCgi().size() != 0)
				{
					std::cout << "THe path is: " << path << std::endl;
					std::string ext;
					if (check_is_cgi(path, let, ext, arguments)== 1)
					{
						if (keep.getLocationsVec()[i].getCgi()[ext] == ".php" || keep.getLocationsVec()[i].getCgi()[ext] == ".py")
							checked_cgi = 1;
						else 
							checked_cgi = 0;
					}
				}
				else 
					checked_cgi = 0;
				if (keep.getLocationsVec()[i].getReturn() != "")
				{
					std::cout << "THe path is entered here " << std::endl;
					std::cout << keep.getLocationsVec()[i].getReturn() << std::endl;
					infa.buffer_to_send = "HTTP/1.1 302 " + keep.getLocationsVec()[i].getReturn() + "\nContent-Type: text/html\nContent-Length: 0\r\n\r\n";
					this->infa.status = 1;
					throw std::exception();					
				}
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
	if (checked_cgi == 1)
	{
		execute_cgi(let, ext, arguments);
		throw std::exception();
	}
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
			if (access(path.c_str(), R_OK) == -1)
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
