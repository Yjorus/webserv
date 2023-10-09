#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "AllHeaders.hpp"
# include "Server.hpp"
# include "Request.hpp"

class Client
{
	int			_client_fd;
	Request		_request;
	time_t		_time;
	Server		_server;
	response	_response;

	public:
		
		Client();
		Client(Server &server);
		// Client(Client const &copy);
		~Client();

		// Client	&operator=(Client const &assign);

		void	setSocketFd(int fd);
		void	setServer(Server &server);

		int			getSocketFd();
		Request		&getRequest();
		void		refreshTime();
		time_t		getTime();
		Server		getServer();
};

#endif