#ifndef MULTIPLEXING_HPP
#define MULTEPLEXING_HPP

#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include "../includes/Servers.hpp"

typedef struct s_client_info {
	socklen_t				address_length;
	struct sockaddr_storage	address;
	int						socket;
	char					request[1024 + 1];
	size_t					received;
	s_client_info			*next;

	s_client_info() : address_length(0), socket(0), request(""), received(0), next(NULL)
	{
        memset(&address, 0, sizeof(address));
    }
}	t_client_info;

int				createListeningSocket(std::string &host, int &port);
void			runServer(Servers &servers);
void			lstadd_front(t_client_info **lst, t_client_info *newLst);
t_client_info	*get_client(int s, t_client_info **clients);
void			drop_client(t_client_info *client, t_client_info **clients);
void			wait_on_clients(int server, t_client_info **clients, fd_set &reads, fd_set &writes);
std::string		get_client_address(t_client_info *client);

#endif