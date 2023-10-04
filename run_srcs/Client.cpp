#include "../inc/Client.hpp"

Client::Client()
{}

// Client::Client(Client const &copy)
// {}

Client::~Client()
{}

// Client	&Client::operator=(Client const &assign)
// {}

void	Client::setSocketFd(int fd) {
	this->_client_fd = fd;
}

void	Client::updateRequest(std::string request)
{
	this->_request_str += request;
}

int	Client::getSocketFd() {
	return(this->_client_fd);
}

std::string	Client::getRequestStr() {
	return(this->_request_str);
}

Request	&Client::getRequest() {
	return (this->_request);
}