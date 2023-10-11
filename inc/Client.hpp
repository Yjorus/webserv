#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "AllHeaders.hpp"
# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"

class Client
{
	int			_client_fd;
	Request		_request;
	time_t		_time;
	Server		_server;
	Response	_response;

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
		Response	&getResponse();
		void		refreshTime();
		time_t		getTime();
		Server		getServer();

		void		clearClient();
};

#endif