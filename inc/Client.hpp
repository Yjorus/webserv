#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "AllHeaders.hpp"
# include "Request.hpp"

class Client
{
	int			_client_fd;
	Request		_request;
	time_t		_time;

	public:
		
		Client();
		// Client(Client const &copy);
		~Client();

		// Client	&operator=(Client const &assign);

		void	setSocketFd(int fd);

		int		getSocketFd();
		Request		&getRequest();
		void		refreshTime();
		time_t		getTime();
};

#endif