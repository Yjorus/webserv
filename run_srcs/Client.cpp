#include "../inc/Client.hpp"

Client::Client()
{}

Client::Client(Client const &copy)
{}

Client::~Client()
{}

Client	&Client::operator=(Client const &assign)
{}

void	Client::setSocketFd(int fd)
{
	this->_client_fd = fd;
}

int	Client::getSocketFd()
{
	return(this->_client_fd);
}