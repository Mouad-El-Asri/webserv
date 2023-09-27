#include "multiplexing.hpp"

void	runServer(Servers &servers)
{
	t_client_info *clients = NULL;
	std::vector<Directives> serversVec = servers.getServersVec();
	std::vector<int> serverSockets(serversVec.size());

	for (size_t i = 0; i < serversVec.size(); i++)
	{
		std::string	host = serversVec[i].getHost();
		int			port = serversVec[i].getListen();
		serverSockets[i] = createListeningSocket(host, port);
	}

	while (true)
	{
		fd_set reads;
		fd_set writes;

		FD_ZERO(&reads);
		FD_ZERO(&writes);
		for (size_t i = 0; i < serverSockets.size(); i++)
		{
			FD_SET(serverSockets[i], &reads);
			wait_on_clients(serverSockets[i], &clients, reads, writes);
			if (FD_ISSET(serverSockets[i], &reads))
			{
				t_client_info *client = get_client(-1, &clients);
				client->socket = accept(serverSockets[i], (struct sockaddr*)&(client->address), &(client->address_length));
				if (client->socket == -1)
					throw std::runtime_error("Error accepting connection");
				std::cout << "New connection from " << get_client_address(client) << "." << std::endl;
			}
		}

		t_client_info *client = clients;
		while(client)
		{
			if (FD_ISSET(client->socket, &reads))
			{
				client->received += recv(client->socket, client->request, sizeof(client->request), 0);
			}

			if (FD_ISSET(client->socket, &writes))
			{

			}
			client = client->next;
		}
	}
	std::cout << "Closing sockets..." << std::endl;
	for (size_t i = 0; i < serverSockets.size(); i++)
		close(serverSockets[i]);
}
