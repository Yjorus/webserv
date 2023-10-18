#include "../inc/RunServer.hpp"

RunServer::RunServer()
{}

RunServer::~RunServer()
{}

void	RunServer::setupServers(std::vector<Server> servers) {
	_servers = servers;
	bool	a = false;
	for (std::vector<Server>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++) {
		a = false;
		for (std::vector<Server>::iterator it2 = this->_servers.begin(); it2 != it; it2++) {
			if (it->getHost() == it2->getHost() && it->getPort() == it2->getPort()) {
				a = true;
				it->setFd(it2->getFd());
			}
		}
		if (a == false)
			it->prepareServer();
	}
}

void	RunServer::connectClient(Server &server) {
	struct sockaddr_in		client_addr;
	socklen_t			client_len = sizeof(client_addr);
	int					client_fd;
	Client				new_client(server);

	if ((client_fd  = accept(server.getFd(), (struct sockaddr *)&client_addr, &client_len)) < 0)
		return ;
	// addToSet(client_fd, _read_fds);
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0) {
		// removeFromSet(client_fd, _read_fds);
		close(client_fd);
		return ;
	}
	addToSet(client_fd, _read_fds);
	// std::cout << "\nNEW CLIENT " << client_fd << std::endl;
	new_client.setSocketFd(client_fd);
	if (_clientmap.count(client_fd) != 0)
		_clientmap.erase(client_fd);
	_clientmap.insert(std::make_pair(client_fd, new_client));
}

void	RunServer::removeClient(int a) {
	if (FD_ISSET(a, &_write_fds))
		removeFromSet(a, _write_fds);
	if (FD_ISSET(a, &_read_fds))
		removeFromSet(a, _read_fds);
	// std::cout << "\nCLOSING CONNECTION TO " << a << std::endl;
	close(a);
	_clientmap.erase(a);
}

void	RunServer::serverLoop() {
	fd_set	write_fds;
	fd_set	read_fds;
	int		a;
	_highest_fd = 0;
	setupSets();
	struct timeval timeout;
	signal(SIGPIPE, SIG_IGN);

	while (1) {
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		read_fds = _read_fds;
		write_fds = _write_fds;
		if ( (a = select(_highest_fd + 1, &read_fds, &write_fds, NULL, &timeout)) < 0)
			exit(1);
		for (int b = 0; b <= _highest_fd; b++) {
			if (FD_ISSET(b, &read_fds) && _servermap.count(b))
				connectClient(_servermap.find(b)->second);
			else if (FD_ISSET(b, &read_fds) && _clientmap.count(b))
				readRequest(b, _clientmap[b]);
			else if (FD_ISSET(b, &write_fds) && _clientmap.count(b))
			{
				int state = _clientmap[b].getResponse().getCgiFlag();
				if (state == 1 && FD_ISSET(_clientmap[b].getResponse().getCgiManager()._pipe_cgi_in[1], &write_fds))
					sendCgiRequest(_clientmap[b], _clientmap[b].getResponse().getCgiManager());
				else if (state == 1 && FD_ISSET(_clientmap[b].getResponse().getCgiManager()._pipe_cgi_out[0], &read_fds))
					handleCgiResponse(_clientmap[b], _clientmap[b].getResponse().getCgiManager());
				else if ((state == 0 || state == 2) && FD_ISSET(b, &write_fds))
					sendResponse(b, _clientmap[b]);
			}
		}
		disconnectTimeout();
	}
}

void	RunServer::disconnectTimeout() {
	for (std::map<int, Client>::iterator it = _clientmap.begin(); it != _clientmap.end(); it++) {
		if ( time(NULL) - it->second.getTime() > 15) {
			removeClient(it->first);
			return ;
		}
	}
}

void	RunServer::addToSet(int a, fd_set &set) {
	FD_SET(a, &set);
	if (a > _highest_fd)
		_highest_fd = a;
}

void	RunServer::removeFromSet(int a, fd_set &set) {
	FD_CLR(a, &set);
	if (a == _highest_fd)
		_highest_fd--;
}

void	RunServer::setupSets() {
	FD_ZERO(&_write_fds);
	FD_ZERO(&_read_fds);
	for (std::vector<Server>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++) {
		if (listen(it->getFd(), 512) < 0)
			exit(1);
		if (fcntl(it->getFd(), F_SETFL, O_NONBLOCK) < 0)
			exit(1);
		addToSet(it->getFd(), _read_fds);
		_servermap.insert(std::make_pair(it->getFd(), *it));
	}
	_highest_fd = _servers.back().getFd();
}

void	RunServer::readRequest(const int &a, Client &client) {
	char	buffer[10000];
	memset(buffer, 0, sizeof(buffer));
	int		read_ret_val = 0; 
	read_ret_val = read(a, buffer, 10000);
	if (read_ret_val <= 0) {
		removeClient(a);
		return ;
	}
	else if (read_ret_val != 0){
		client.refreshTime();
		client.getRequest().parseRequest(buffer, read_ret_val);
		memset(buffer, 0, sizeof(buffer));
	}
	if (client.getRequest().isFinished() || client.getRequest().getErrorCode()) {
		setCorrectServerName(client);
		client.getResponse().initializeResponse(client.getRequest());
		client.getResponse().buildResponse();
		if (client.getResponse().getCgiFlag()) {
			handleCgiRequest(client);
			addToSet(client.getResponse().getCgiManager()._pipe_cgi_in[1], _write_fds);
			addToSet(client.getResponse().getCgiManager()._pipe_cgi_out[0], _read_fds);
		}
		removeFromSet(a, _read_fds);
		addToSet(a, _write_fds);
	}
}

void	RunServer::handleCgiRequest(Client & client) {
	if (client.getRequest().getBody().size() == 0) {
		std::string hold;
		std::fstream	file(client.getResponse().getCgiManager().getPath().c_str());
		std::stringstream stream;
		stream << file.rdbuf();
		hold = stream.str();
		client.getRequest().setBody(hold);
	}
}

void	RunServer::setCorrectServerName(Client &client) {
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++) {
		if (it->getHost() == client.getServer().getHost() && it->getPort() && client.getServer().getPort() \
		&& it->getServerName() == client.getRequest().getServerName()) {
			client.setServer(*it);
			return ;
		}
	}
}

void	RunServer::sendResponse(const int &a, Client &client) {
	int	write_return;
	std::string	response = client.getResponse().getResponse();
	if (response.length() >= 10000)
		write_return = write(a, response.c_str(), 10000);
	else
		write_return = write(a, response.c_str(), response.length());
	if (write_return < 0)
		removeClient(a);
	else if (write_return == 0 || (size_t)write_return == response.length()) {
		if (client.getRequest().getErrorCode() || client.getRequest().keepAlive() == false || client.getResponse().getCgiFlag()) // client.getRequest().keepAlive() == false || client.getResponse().getCgiState()
				removeClient(a);
		else {
			removeFromSet(a, _write_fds);
			addToSet(a, _read_fds);
			client.clearClient();
		}
	}
	else {
		client.refreshTime();
		client.getResponse().cutResponse(write_return);
	}
}

void	RunServer::sendCgiRequest(Client &client, CgiManager &manager) {
	std::string &body = client.getRequest().getBody();
	int a;
	if (body.size() == 0)
		a = 0;
	else if (body.size() >= 10000)
		a = write(manager._pipe_cgi_in[1], body.c_str(), 10000);
	else
		a = write(manager._pipe_cgi_in[1], body.c_str(), body.length());
	if (a < 0) {
		removeFromSet(manager._pipe_cgi_in[1], _write_fds);
		close(manager._pipe_cgi_in[1]);
		close(manager._pipe_cgi_out[1]);
		client.getResponse().setCgiErrorResponse(500);
	}
	else if (a == 0 || (size_t) a == body.length()) {
		removeFromSet(manager._pipe_cgi_in[1], _write_fds);
		close(manager._pipe_cgi_in[1]);
		close(manager._pipe_cgi_out[1]);
	}
	else {
		client.refreshTime();
		body = body.substr(a);
	}
}

void	RunServer::handleCgiResponse(Client &client, CgiManager &manager) {
	char	buffer[20000];
	memset(buffer, 0, sizeof(buffer));
	int		read_ret_val = 0; 
	read_ret_val = read(manager._pipe_cgi_out[0], buffer, 20000);
	if (read_ret_val < 0) {
		removeFromSet(manager._pipe_cgi_out[0], _read_fds);
		close(manager._pipe_cgi_in[0]);
		close(manager._pipe_cgi_out[0]);
		client.getResponse().setCgiFlag(2);
		client.getResponse().setCgiErrorResponse(500);
		return ;
	}
	else if (read_ret_val == 0){
		removeFromSet(manager._pipe_cgi_out[0], _read_fds);
		close(manager._pipe_cgi_in[0]);
		close(manager._pipe_cgi_out[0]);
		int status;
		waitpid(manager.getPid(), &status, 0);
		if (WEXITSTATUS(status) != 0)
			client.getResponse().setCgiErrorResponse(502);
		client.getResponse().setCgiFlag(2);
		return ;
	}
	else {
		client.refreshTime();
		client.getResponse().updateResponse(buffer, read_ret_val);
		memset(buffer, 0, sizeof(buffer));
	}
}
