#include "multiplexing.hpp"

void	runServer(Servers &servers)
{
	t_client_info *clients = NULL;
	std::vector<Directives> serversVec = servers.getServersVec();
	std::vector<int> serverSockets(serversVec.size());

	for (size_t i = 0; i < serversVec.size(); i++)
	{
		std::string	host = serversVec[i].getServerName();
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
			FD_SET(serverSockets[i], &writes);
		}
	}
}
