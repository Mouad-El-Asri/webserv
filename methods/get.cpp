//* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouzanb <abouzanb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 03:56:24 by abouzanb          #+#    #+#             */
/*   Updated: 2023/10/09 06:04:24 by abouzanb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get.hpp"
#include "../Multiplexing/multiplexing.hpp"
#define P 8081



// hndling cgi
void set_headerto_send(info& inf, std::string headers)
{
	inf.buffer_to_send = headers;
	inf.temp->close();
	delete inf.temp;
	inf.status = 1;
	inf.is_hinged = 0;
	close(inf.pipe);
}


void read_cgi_output(info& infa) {
    static size_t size = 0;
	infa.there_cgi = 1;
	struct stat st;
	size_t readed = 0;
    char Buff[1501];
    std::string buf;
	std::string body;
	std::stringstream ss;
	std::stringstream sss;
	std::string namefile;
    std::string str;
	if (infa.first_enter == 0)
	{
		sss << infa.socket + 55;
		infa.filename = sss.str() + ".txt";
		infa.temp = new std::ofstream(infa.filename.c_str());
	}
	std::string content_type = "text/html";
	std::string content_lenght;
	std::string headers;
    if (infa.first_enter == 0) {
					infa.first_enter = 1;
					readed = read(infa.pipe, Buff, 1500);
					Buff[readed] = '\0';
					if (readed == 0)
						return set_headerto_send(infa , "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n\r\n");
					else if (readed < 0)
						return set_headerto_send(infa , "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: 25\r\n\r\n<h1>Internel server error<h1>");
					buf = Buff;
					if (buf.find("\r\n\r\n") != std::string::npos)
					{
						headers = buf.substr(0, buf.find("\r\n\r\n"));
						if (headers.find("Content-Type: ") != std::string::npos)
							content_type = headers.substr(headers.find("Content-Type: ") + 14, headers.find("\r\n") -  headers.find("Content-Type: ") - 14);
						else
							content_type = "text/html";
						if (headers.find("Content-Length: ") == std::string::npos)
						{
							infa.no_content_length = -1;
						
						}else {
							content_lenght = headers.substr(headers.find("Content-Length: ") + 16, headers.find("\r\n") -  headers.find("Content-Length: ") - 16);
							size = std::atoi(content_lenght.c_str());
							if (size == 0)
								return set_headerto_send(infa , "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n");
						}
						body = buf.substr(buf.find("\r\n\r\n") + 4, buf.size() - buf.find("\r\n\r\n") - 4);
					}
					else 
					{
						infa.no_content_length = -1;
						infa.buffer_to_send = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
						body = buf;
					}
					try {
						if (readed > size && infa.no_content_length != -1)
						{
							*infa.temp << body.substr(0, size);
							infa.file = new std::ifstream(infa.filename.c_str());
							infa.size = size;
							set_headerto_send(infa, "HTTP/1.1 200 OK\r\nContent-Type: " + content_type + "\r\nContent-Length: " + content_lenght + "\r\n\r\n");
							return ;
						}
						else
						{
							*infa.temp << body;
							size -= readed;
						}
					}
					catch(std::exception &e)
					{
						std::cout << "\e[91mError : \e[42man execption is thrown => : " << e.what() << "\e[0m" << std::endl;
						return set_headerto_send(infa , "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
					}
		}
		else if (infa.first_enter == 1)
		{
			readed = read(infa.pipe, Buff, 1500);
			Buff[readed] = '\0';
			if (readed == 0)
			{
				infa.file = new std::ifstream(infa.filename.c_str());
				infa.temp->close();
				if (infa.no_content_length == -1)
				{
					stat(infa.filename.c_str(), &st);
					ss << st.st_size;
					content_lenght = ss.str();
				}
				set_headerto_send(infa, "HTTP/1.1 200 OK\r\nContent-Type: "+ content_type +"\r\nContent-Length: " + content_lenght + "\r\n\r\n");
				return ;
			}
			else if (readed < 0)
				return set_headerto_send(infa , "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: 25\r\n\r\n<h1>Internel server error<h1>");
			else if (readed  >= size && infa.no_content_length != -1)
			{
				*infa.temp << body.substr(0, size);
				infa.file = new std::ifstream(infa.filename.c_str());
				if (infa.file->is_open() == false)
					std::cout << "file is not open" << std::endl;
				set_headerto_send(infa, "HTTP/1.1 200 OK\r\nContent-Type: "+  content_type  +"\r\nContent-Length: " + content_lenght + "\r\n\r\n");
				return ;			
			}
			else 
			{
				*infa.temp << body;
				size -= readed;
			}
		}
}


void method_get::execute_cgi(std::string& path, std::string& arguments, std::string& run_it) {
    int fd[2];
    pid_t pid;
    char *argv[4];
    std::string str;

    char *envp[5];
    struct stat st;

    if (stat(path.c_str(), &st) == -1) {
        set_error("404");
        throw std::runtime_error("\e[91mError: The file is not found. Response with 404\e[0m");
    }

    if (access(path.c_str(), R_OK) == -1) {
        set_error("403");
        throw std::runtime_error("\e[91mError: The file is not readable. Response with 403\e[0m");
    }

    pipe(fd);
    pid = fork();

    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], 1);
        argv[0] = strdup(run_it.c_str());
        argv[1] = strdup(path.c_str());
        argv[2] = NULL;
        envp[0] = strdup("REQUEST_METHOD=GET");
        str = "PATH_INFO=" + path;
        envp[1] = strdup(str.c_str());
        if (!arguments.empty()) {
            envp[2] = strdup(arguments.c_str());
            envp[3] = NULL;
        } else {
            envp[2] = NULL;
        }
        execve(run_it.c_str(), argv, envp);
        exit(0);
    } else {
        close(fd[1]);

        int status;
		time(&infa.start);
        infa.waitpid_ret = waitpid(pid, &status, WNOHANG);

        if (infa.waitpid_ret == -1) {
            set_error("500");
            throw std::runtime_error("\e[91mError: The CGI is not executed. Response with 500\e[0m");
        } else if (infa.waitpid_ret == pid) {
            read_cgi_output(infa);
            throw std::runtime_error("\e[91mThe CGI is executed \e[0m");
        } else if (infa.waitpid_ret == 0) {
            infa.pipe = fd[0];
            infa.pid = pid;
            infa.is_hinged = 1;
            throw std::runtime_error("\e[91mThe CGI is hung\e[0m");
        }
    }
}

int check_is_cgi(std::string& path, std::string& real_path, std::string& extansion, std::string& arguments)
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
		else if (path.size() >= 4 && path.substr(path.size() - 4) == ".php")
		{
			extansion = ".php";
			real_path = path;
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
		else if (path.size() >= 3 && path.substr(path.size() - 3) == ".py")
		{
			extansion = ".py";
			real_path = path.substr(0, path.find_last_of(".py") + 3);
			arguments = "";
			return 1;
		}
	}
	return 0;
}


// handling the request

void method_get::check_location()
{
	int forbidden = 0;
	std::string let;
	size_t i = 0;
	int max_lenght = 0;
	std::string ext;
	std::string arguments;
	int redirection = 0;
	std::stringstream ss;
	int checked_cgi = 0;
	std::string cgi_script;
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

				if (keep.getLocationsVec()[i].getCgi().size() != 0)
				{
					std::string ext;
					if (check_is_cgi(path, let, ext, arguments) == 1)
					{
						
						if (keep.getLocationsVec()[i].getCgi()[ext] == "/usr/bin/php" || keep.getLocationsVec()[i].getCgi()[ext] == "/bin/python3")
						{
							
							checked_cgi = 1;
							cgi_script = keep.getLocationsVec()[i].getCgi()[ext];
						}
						else 
							checked_cgi = 0;
					}
				}
				else 
					checked_cgi = 0;
				if (keep.getLocationsVec()[i].getReturn().size())
				{
					infa.buffer_to_send = "HTTP/1.1 "+ keep.getLocationsVec()[i].getReturn()[0] + "" + "Found\r\nLocation: " + keep.getLocationsVec()[i].getReturn()[1] + "\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n";
					redirection = 1;			
				}
				else
				{
					redirection = 0;
				}
				is_checked = 1;
				forbidden = 0;
			}
			else
				forbidden = 1;
		}
		i++;
	}
	infa.path = path;
	if (forbidden == 1)
	{
		set_error("405");

		throw std::runtime_error("\e[91mError: The method is not allowed. response with 403\e[0m");
	}
	if (redirection == 1)
	{
		throw std::runtime_error("\e[91mError: The location is redirected. response with 302\e[0m");
	}
	if (checked_cgi == 1)
	{
		execute_cgi(let,  arguments, cgi_script);
		std::cout << "\e[96mGET : \e[42mThe cgi is executed\e[0m" << std::endl;
	}
	else if (check_is_cgi(path, let, ext, arguments) == 1)
	{
		this->path = let;
		file_handling();
	}
	if (is_checked == 0)
	{
		set_error("404");
		throw std::runtime_error("\e[91mError: The location is not found. response with 404\e[0m");
	}
	
}
void method_get::get_check_path()
{
	struct stat st;
	try {
			check_location();
			if (stat(path.c_str() , &st) == 0)
			{
					if (access(path.c_str(), R_OK) == -1)
					{
						set_error("403");
						throw std::runtime_error("\e[91mError: The file is not readable. response with 403\e[0m");
					}
					this->size = st.st_size;
					infa.size = st.st_size;
					if (S_ISDIR(st.st_mode))
						folder_handling();
					else
					{
						file_handling();
						throw 	std::runtime_error("\e[42mFile is requested , resopnse with file\e[0m");
					}
			}
			else
			{
				set_error("404");
				throw std::runtime_error("\e[91mError: The file is not found. response with 404\e[0m");
			}
	}
	catch (const std::exception& e)
	{
		std::cout << "\e[36mGET METHOD  : "  << e.what() << std::endl;
	}
}

void method_get::set_extasion()
{
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
	extansion_handling[".php"] = moth + "Content-Type: text/html\nContent-Length: ";
	extansion_handling[".py"] = moth + "Content-Type: text/html\nContent-Length: ";
	extansion_handling["404"] = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ";
	extansion_handling["403"] = "HTTP/1.1 403 Forbidden\nContent-Type: text/html\nContent-Length: ";
	extansion_handling["500"] = "HTTP/1.1 500 Internal Server Error\nContent-Type: text/html\nContent-Length: ";
	extansion_handling["def"] = "HTTP/1.1 200 OK\nContent-Type: application/octet-stream\nContent-Length: ";
	extansion_handling["405"] = "HTTP/1.1 405 Method Not Allowed\nContent-Type: text/html\nContent-Length: ";
	extansion_handling["500"] = "HTTP/1.1 500 Internal Server Error\nContent-Type: text/html\nContent-Length: ";
}

method_get::method_get(Directives& k , std::string url, info &inf) : infa(inf) , keep(k)
{
	this->keep = k;
	std::string route = k.getRoot();
	this->path = route + url;
	auto_index = k.getAutoIndex();
	this->url = url;
	infa.path = path;
	infa.error_pages = k.getErrorPages();
	this->erros_page = k.getErrorPages();
	this->set_extasion();
}


void ft_get(Directives &data, std::string url,  info &socket)
{
	if (socket.was_read == 1)
		return ;
	method_get get(data, url,  socket);
	get.get_check_path();
	socket.was_read = 1;
}






// response part 
void send_file_in_response(info &clientes, t_client_info *client_write , t_client_info **clients, fd_set &reads, fd_set &writes)
{
	char bu[1025];
	clientes.file->read(bu, 1024);
	int reading = clientes.file->gcount();
	std::cout << "\e[96mGET : \e[42mSending data to the socket " << clientes.socket << "\e[0m" << std::endl;
	int sending   = send(clientes.socket, bu, reading, 0) ;
	if	(sending < 0)
	{
		std::cout << "\e[91mGet : \e[42mError : an error occured with the socket " << clientes.socket <<  ", while sending the file\e[0m" << std::endl;
		if (clientes.file)
		{
			clientes.file->close();
			if (clientes.there_cgi == 1)
				std::remove(clientes.filename.c_str());
			delete clientes.file;
		}
		drop_client(client_write, clients, reads, writes);
		return ;
	}
	if (sending == 0)
	{
		std::cout << "\e[91mGet : \e[42mThe file is empty the send function returns 0  \e[0m" << std::endl;
		if (clientes.file)
		{
			clientes.file->close();
			if (clientes.there_cgi == 1)
				std::remove(clientes.filename.c_str());
			delete clientes.file;
		}
		drop_client(client_write, clients, reads, writes);
		return ;
	}
}

void send_header_in_response(info &clientes, t_client_info *client_write , t_client_info **clients, fd_set &reads, fd_set &writes)
{
	std::cout << "\e[96mGET : \e[42mSending header to the socket " << clientes.socket << "\e[0m" << std::endl;
	int i =  send(clientes.socket, clientes.buffer_to_send.c_str(), clientes.buffer_to_send.size(), 0);
	if (i < 0)
	{
		std::cout << "\e[91mGet : \e[42mError : an error occured with the socket " << clientes.socket <<  ", while sending the header\e[0m" << std::endl;
		if (clientes.file)
		{
			clientes.file->close();
			if (clientes.there_cgi == 1)
				std::remove(clientes.filename.c_str());
			delete clientes.file;
		}
		drop_client(client_write, clients, reads, writes);
		return ;
	}
	if (i == 0)
	{
		std::cout << "\e[91mGet : \e[42mThe header is empty the send function returns 0  \e[0m" << std::endl;
		if (clientes.file)
		{
			clientes.file->close();		
			if (clientes.there_cgi == 1)
				std::remove(clientes.filename.c_str());
			delete clientes.file;
		}
		drop_client(client_write, clients, reads, writes);
		return ;
	}
	clientes.status = 0;	
}


void	get_response(info &clientes, t_client_info *client_write , t_client_info **clients, fd_set &reads, fd_set &writes)
{
	if (clientes.is_hinged == 1)
	{
		int status;
		clientes.waitpid_ret = waitpid(clientes.pid, &status, WNOHANG);
		if (clientes.waitpid_ret == clientes.pid)
		{
				std::cout << "\e[96mGET : \e[42mHnadle cgi in response\e[0m" << std::endl;
				read_cgi_output(clientes);
		}
		else if (clientes.first_enter == 1)
		{
				std::cout << "\e[96mGET : \e[42mHnadle cgi in response\e[0m" << std::endl;
				read_cgi_output(clientes);			
		}
		time_t end;
		time(&end);
		if (difftime(end, clientes.start) >= 5)
		{
			set_error(clientes.error_pages, clientes, "504", "HTTP/1.1 504 Gateway Timeout\r\nContent-Type: text/html\r\nContent-Length: " );
			clientes.status = 1;
			clientes.is_hinged = 0;
			kill(clientes.pid, SIGKILL);
		}
		if (WIFEXITED(status) != 0)
		{
			if (WEXITSTATUS(status) != 0)
			{
			set_error(clientes.error_pages, clientes, "502", "HTTP/1.1 502 Bad Gateway\r\nContent-Type: text/html\r\nContent-Length: " );
			clientes.status = 1;
			clientes.is_hinged = 0;
			}
		}
		return ;
	}
	if (clientes.status == 1)
		send_header_in_response(clientes, client_write, clients, reads, writes);
	else if (clientes.file && clientes.file->eof() == false) 
		send_file_in_response(clientes, client_write, clients, reads, writes);
	else
	{
		if (clientes.file)
		{
			clientes.file->close();
			if (clientes.there_cgi == 1)
				std::remove(clientes.filename.c_str());
			delete clientes.file;
		}
		std::cout << "\e[96mGET : \e[41mClosing the socket " << clientes.socket << "Method GET is ended here . \e[0m" << std::endl;
		drop_client(client_write, clients, reads, writes);
		return ;
	}
}