#ifndef MULTIPLEXING_HPP
#define MULTEPLEXING_HPP

#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include "../includes/Servers.hpp"

class Request
{
public:
    std::string fst_line;
    std::string *method;
    std::string path_dir;
    std::string file_path;
	std::string *filename;
    Request();
    ~Request();
};

typedef struct s_client_info {
	socklen_t				address_length;
	struct sockaddr_storage	address;
	int						socket;
	char					request[1024];
	ssize_t					received;
	size_t					all_received;
	bool					is_chunked_encoding;
    bool					is_content_length;
	bool					is_multipart;
	size_t					times;
	size_t					bl;
	size_t					binary_data_start;
	bool					finished;
	std::string				req_body;
	s_client_info			*next;
	Request					header;

	s_client_info() : address_length(0), socket(0), request(""), received(0), all_received(0), is_chunked_encoding(false), is_content_length(false), is_multipart(false), times(0), bl(0), next(NULL)
	{
        memset(&address, 0, sizeof(address));
		memset(&request, 0, sizeof(request));
    }
}	t_client_info;

int				createListeningSocket(std::string &host, int &port);
void			runServer(Servers &servers);
void			lstadd_front(t_client_info **lst, t_client_info *newLst);
t_client_info	*get_client(int s, t_client_info **clients);
void			drop_client(t_client_info *client, t_client_info **clients);
void			wait_on_clients(int &maxSocket, t_client_info **clients, fd_set &reads, fd_set &writes, fd_set &tempReads, fd_set &tempWrites);
std::string		get_client_address(t_client_info *client);

#endif