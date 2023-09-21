#include "multiplexing.hpp"

void	lstadd_front(t_client_info **lst, t_client_info *newLst)
{
	newLst->next = *lst;
	*lst = newLst;
}

t_client_info	*get_client(int s, t_client_info **clients)
{
	t_client_info *client = *clients;

	while(client)
	{
		if (client->socket == s)
			return (client);
		client = client->next;
	}

	t_client_info* newClient = new t_client_info;

	newClient->address_length = sizeof(newClient->address);
	lstadd_front(clients, newClient);

	return (newClient);
}

void	drop_client(t_client_info *client, t_client_info **clients)
{
	close(client->socket);
	t_client_info **p = clients;
	while(*p)
	{
		if (*p == client)
		{
			*p = client->next;
			free(client);
			return ;
		}
		p = &(*p)->next;
	}
	throw std::runtime_error("Client not found.");
}

void wait_on_clients(int server, t_client_info **clients, fd_set &reads, fd_set &writes)
{
	t_client_info	*client;
	int				max_socket;
	int				result;

	max_socket = server;
	client = *clients;

	while (client)
	{
		FD_SET(client->socket, &reads);
		FD_SET(client->socket, &writes);
		if (client->socket > max_socket)
			max_socket = client->socket;
		client = client->next;
	}

	result = select(max_socket + 1, &reads, &writes, NULL, NULL);
	if (result == -1)
		throw std::runtime_error("select failed with an error.");
}
