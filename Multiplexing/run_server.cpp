#include "../Request/POST/ft_Post.hpp"

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

	fd_set	reads;
	fd_set	writes;
	fd_set	tempReads;
	fd_set	tempWrites;
	int		maxSocket;

	FD_ZERO(&reads);
	FD_ZERO(&writes);
	maxSocket = 0;

	for (size_t i = 0; i < serverSockets.size(); i++)
	{
		FD_SET(serverSockets[i], &reads);
		if (serverSockets[i] > maxSocket)
			maxSocket = serverSockets[i];
	}

	while (true)
	{
		wait_on_clients(maxSocket, &clients, reads, writes, tempReads, tempWrites);
		for (size_t i = 0; i < serverSockets.size(); i++)
		{
			if (FD_ISSET(serverSockets[i], &tempReads))
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
			if (FD_ISSET(client->socket, &tempReads))
			{
				std::cout << "1" << std::endl;
				client->received += recv(client->socket, client->request, 1024, 0);
				std::cout << std::string(client->request) << std::endl;
				if (client->received == -1)
				{
					std::cerr << "Error reading from client " << get_client_address(client) << "." << std::endl;
					drop_client(client, &clients);
				}
				else if (client->received == 0)
				{
					std::cerr << "Client socket " << client->socket << " closed." << std::endl;
					drop_client(client, &clients);
				}
				else
                    std::cout << "Received " << client->received << " bytes from client " << get_client_address(client) << "." << std::endl;
				handle_Post(client);
			}
			client = client->next;
		}

		t_client_info *client_write = clients;
		while(client_write)
		{
			if (FD_ISSET(client_write->socket, &tempWrites))
			{
				std::cout << "2" << std::endl;
				exit(1);
			}
			client_write = client_write->next;
		}
	}
	std::cout << "Closing sockets..." << std::endl;
	for (size_t i = 0; i < serverSockets.size(); i++)
		close(serverSockets[i]);
}
