#ifndef CLIENT_INFO_HPP
#define CLIENT_INFO_HPP

#include "run_server.hpp"

#define MAX_REQUEST_SIZE 2047

class ClientInfo
{
	private:
		socklen_t				address_length;
		struct sockaddr_storage	address;
		int						socket;
		std::string				request;
		int						received;
		ClientInfo*				next;

	public:
		ClientInfo();

		socklen_t			getAddressLength() const;
		sockaddr_storage	getAddress() const;
		int					getSocket() const;
		int					getReceived() const;
		const std::string&	getRequest() const;
		ClientInfo*			getNext() const { return next; }

		void	setAddressLength(const socklen_t length) { address_length = length; }
		void	setAddress(const sockaddr_storage addr) { address = addr; }
		void	setSocket(const int sock) { socket = sock; }
		void	setRequest(const std::string& req) { request = req; }
		void	setReceived(const int recv) { received = recv; }
		void	setNext(ClientInfo* nxt) { next = nxt; }
};

#endif