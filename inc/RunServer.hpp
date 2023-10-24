#ifndef RUNSERVER_HPP
# define RUNSERVER_HPP

# include "AllHeaders.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include "CgiManager.hpp"

class	RunServer
{
	fd_set	_read_fds;
	fd_set	_write_fds;
	int		_highest_fd;
	std::vector<Server>	_servers;
	std::map<int, Client>	_clientmap;
	std::map<int, Server>	_servermap;

	public:
		RunServer();
		~RunServer();

		void	setupServers(std::vector<Server> servers);
		void	connectClient(Server &server);
		void	removeClient(int a);
		void	disconnectTimeout(fd_set &set);
		void	serverLoop();
		void	addToSet(int a, fd_set &set);
		void	removeFromSet(int a, fd_set &set);
		void	setupSets();
		void	readRequest(const int &a, Client &client);
		void	setCorrectServerName(Client &client);
		void	sendResponse(const int &a, Client &client);
		void	sendCgiRequest(Client &client, CgiManager &manager);
		void	handleCgiResponse(Client &client, CgiManager &manager);
		void	handleCgiRequest(Client & client);
};

#endif