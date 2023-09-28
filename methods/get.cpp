#include "get.hpp"
#define P 8081
 

void method_get::check_location()
{
	int forbidden = 0;
	size_t i = 0;
	int max_lenght = 0;
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
				path = route + url.substr(size, url.size());
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
	check_location();
	}
	catch (std::exception &e)
	{
		return ;
	}
	std::cout << "path: " << path << std::endl;
	if (stat(path.c_str() , &st) == 0)
	{
		try{
		
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


method_get::method_get(Directives k , std::string url, info &inf) : infa(inf)
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


void ft_get(Directives data, std::string url,  info &socket)
{
	// if (socket.was_read == 1)
	// 	return ;
std::cout << url << std::endl;
	method_get get(data,url,  socket);
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

void get_response(std::vector<info> &clientes, size_t& i , fd_set &writefds, fd_set &readfds)
{
	char bu[1025];
	if (clientes[i].status == 1)
	{
		std::cout << "|||"<< clientes[i].buffer_to_send << "|| "<< std::endl;
		if (send(clientes[i].socket, clientes[i].buffer_to_send.c_str(), clientes[i].buffer_to_send.size(), 0) <= 0)
		{
			std::cout  <<"FAILED TO SEND" << std::endl;
			close_socket(clientes, i, writefds , readfds);
			return ;
		}
			clientes[i].status = 0;
	}
	if (clientes[i].file &&  !clientes[i].file->eof())
	{
		clientes[i].file->read(bu, 1024);
		int count = clientes[i].file->gcount();
		if (count <= 0)
		{
			close_socket(clientes, i, writefds , readfds);
			return ;
		}
		if (send(clientes[i].socket, bu, count, 0) <= 0)
		{
			close_socket(clientes, i, writefds , readfds);
			return ;
		}		
	}
	else
		close_socket(clientes, i, writefds , readfds);
}

int main()
{
	try {
        std::cout << "akojha   aloha "<< std::endl;
	std::ifstream conf("/nfs/homes/abouzanb/Desktop/cloned_webserver/conf/default.conf");
	Servers 	servers;
	skipEmptyLinesAndCheckServerBlock(conf, true, servers);
	parse_servers(servers);
    // Directives a = servers.getServersVec()[0];
	char bu[1024];
	std::map<std::string, std::string> data;
	std::vector<info> clientes; 
	struct sockaddr_in addr;
	fd_set readfds,  writefds;
	int socket_cl; 
	int opt = 1;
	signal(SIGPIPE, SIG_IGN);

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	int server = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(P);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(server, (sockaddr *)&addr, sizeof(addr));
	listen(server, 20);

	data["rote"] = servers.getServersVec()[0].getRoot();

    FD_SET(server, &readfds);
	FD_SET(server, &writefds);
	int max_fd = server;
	while (1)
	{
	 fd_set temp_read = readfds; 
	 fd_set temp_write = writefds;
			
	 int ret = select(max_fd + 1, &temp_read, &temp_write, NULL, NULL);


	 if (ret < 0)
	 {
		 std::cout << "error select" << std::endl;
		 exit(1);
	 }
	 if (FD_ISSET(server, &temp_read))
	 {
		socket_cl = accept(server , 0, 0);
		if (socket_cl > max_fd)
			max_fd = socket_cl;
		FD_SET(socket_cl, &temp_read);
		FD_SET(socket_cl, &readfds);
		FD_SET(socket_cl, &writefds);
		FD_SET(socket_cl, &temp_write);
		clientes.push_back(info(socket_cl));
	}
	
	for (size_t i = 0; i < clientes.size(); i++)
	{
		if (FD_ISSET(clientes[i].socket, &temp_read))
		{
            char bu[1025];
		    int b = recv(clientes[i].socket, bu, 1024, 0);
			if (b <= 0)
			{
				close_socket(clientes, i, writefds , readfds);
				continue ;
			}
	    	bu[b] = '\0';
			std::string s;
		  	s = bu;
		 	try {
					//check which methid is it
					if (s.find("GET") == 0)
					{
		  			data["path"] = s.substr(4, s.find("HTTP") - 5);
		  				ft_get(servers.getServersVec()[0], data["path"], clientes[i]);
					}
					else if (s.find("DELETE") == 0)
					{
						try {
						data["path"] = s.substr(7, s.find("HTTP") - 8);								
						ft_delete(servers.getServersVec()[0], data["path"], clientes[i]);
						}
						catch (std::exception &e)
						{
							std::cout << "error ft_delete   : " << e.what() << std::endl; 
						}
					
					}
				}
			catch (std::exception &r){
			// close_socket(clientes, i, readfds);
		  	}

		}
		else if (FD_ISSET(clientes[i].socket, &temp_write))
		{
			bzero(bu,sizeof(bu));
			get_response(clientes, i, writefds, readfds);
		}
	}
	}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}