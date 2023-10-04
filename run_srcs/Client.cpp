#include "../inc/Client.hpp"

Client::Client()
{
	_time = time(NULL);
}

// Client::Client(Client const &copy)
// {}

Client::~Client()
{}

// Client	&Client::operator=(Client const &assign)
// {}

void	Client::setSocketFd(int fd) {
	this->_client_fd = fd;
}

int	Client::getSocketFd() {
	return(this->_client_fd);
}

Request	&Client::getRequest() {
	return (this->_request);
}

void	Client::refreshTime() {
	this->_time = time(NULL);
}

time_t	Client::getTime() {
	return (this->_time);
}
