#include "../inc/Client.hpp"

Client::Client()
{
	_time = time(NULL);
}

Client::Client(Server &server)
{
	_server = server;
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

void	Client::setServer(Server &server) {
	this->_server = server;
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

Server	Client::getServer() {
	return (this->_server);
}

time_t	Client::getTime() {
	return (this->_time);
}

