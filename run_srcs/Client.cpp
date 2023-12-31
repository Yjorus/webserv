/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjorus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 13:30:12 by yjorus            #+#    #+#             */
/*   Updated: 2023/10/24 13:30:13 by yjorus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client() {
	_time = time(NULL);
}

Client::Client(Server &server) {
	_time = time(NULL);
	_request.setAddress(server.getHost());
	_response.setServer(server);
}

Client::Client(Client const &copy)
{
	*this = copy;
}

Client::~Client()
{}

Client	&Client::operator=(Client const &assign) {
	if (this != &assign) {
		this->_client_fd = assign._client_fd;
		this->_request = assign._request;
		this->_time = assign._time;
		this->_response = assign._response;
	}
	return (*this);
}

void	Client::setSocketFd(int fd) {
	this->_client_fd = fd;
}

void	Client::setServer(Server &server) {
	this->_response.setServer(server);
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
	return (this->_response.getServer());
}

time_t	Client::getTime() {
	return (this->_time);
}

Response &Client::getResponse(){
	return (this->_response);
}

void	Client::clearClient() {
	this->getRequest().clearRequest();
	this->getResponse().clearResponse();
}

