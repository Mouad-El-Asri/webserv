#include "client_info.hpp"

ClientInfo::ClientInfo() : address_length(0), socket(0), received(0), next(NULL)
{
	request.reserve(MAX_REQUEST_SIZE + 1);
}

socklen_t	ClientInfo::getAddressLength() const
{
	return (this->address_length);
}

sockaddr_storage	ClientInfo::getAddress() const
{
	return (this->address);
}

int	ClientInfo::getSocket() const
{
	return (this->socket);
}

int	ClientInfo::getReceived() const
{
	return (this->received);
}

const std::string&	ClientInfo::getRequest() const
{
	return (this->request);
}

void	ClientInfo::setAddressLength(const socklen_t length)
{
	this->address_length = length;
}

void	ClientInfo::setAddress(const sockaddr_storage addr)
{
	this->address = addr;
}

void	ClientInfo::setSocket(const int sock)
{
	this->socket = sock;
}

void	ClientInfo::setRequest(const std::string& req)
{
	this->request = req;
}

void	ClientInfo::setReceived(const int recv)
{
	this->received = recv;
}

void	ClientInfo::setNext(ClientInfo* nxt)
{
	this->next = nxt;
}
