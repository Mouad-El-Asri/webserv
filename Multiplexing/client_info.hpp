#ifndef CLIENT_INFO_HPP
#define CLIENT_INFO_HPP

#include "run_server.hpp"

#define MAX_REQUEST_SIZE 2047

class ClientInfo
{
	private:
		socklen_t			address_length;
		sockaddr_storage	address;
		int					socket;
		std::string			request;
		int					received;
		ClientInfo*			next;

	public:
		ClientInfo() : address_length(0), socket(0), received(0), next(NULL) {
			request.reserve(MAX_REQUEST_SIZE + 1);
		}

		socklen_t			getAddressLength() const { return address_length; }
		sockaddr_storage	getAddress() const { return address; }
		int					getSocket() const { return socket; }
		int					getReceived() const { return received; }
		const std::string&	getRequest() const { return request; }
		ClientInfo*			getNext() const { return next; }

		void	setAddressLength(socklen_t length) { address_length = length; }
		void	setAddress(sockaddr_storage addr) { address = addr; }
		void	setSocket(int sock) { socket = sock; }
		void	setRequest(const std::string& req) { request = req; }
		void	setReceived(int recv) { received = recv; }
		void	setNext(ClientInfo* nxt) { next = nxt; }
};

#endif