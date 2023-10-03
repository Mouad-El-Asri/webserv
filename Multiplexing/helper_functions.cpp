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
	// close(client->socket);
	t_client_info **p = clients;
	while(*p)
	{
		if (*p == client)
		{
			*p = client->next;
			return ;
		}
		p = &(*p)->next;
	}
	throw std::runtime_error("Client not found.");
}

void wait_on_clients(int &maxSocket, t_client_info **clients, fd_set &reads, fd_set &writes, fd_set &tempReads, fd_set &tempWrites)
{
	t_client_info	*client = *clients;
	int				result;

	FD_ZERO(&tempReads);
	FD_ZERO(&tempWrites);

	while (client)
	{
		if (!FD_ISSET(client->socket, &reads))
			FD_SET(client->socket, &reads);

		if (client->socket > maxSocket)
			maxSocket = client->socket;
		client = client->next;
	}

	for (int i = 0; i < FD_SETSIZE; i++)
	{
		if (FD_ISSET(i, &reads))
			FD_SET(i, &tempReads);
		if (FD_ISSET(i, &writes))
			FD_SET(i, &tempWrites);
	}

	result = select(maxSocket + 1, &tempReads, &tempWrites, NULL, NULL);
	if (result == -1)
		throw std::runtime_error("select failed with an error.");
}

std::string	get_client_address(t_client_info *client)
{
	char address_buffer[NI_MAXHOST];

	getnameinfo((struct sockaddr*)&client->address, client->address_length, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
	return (std::string(address_buffer));
}
