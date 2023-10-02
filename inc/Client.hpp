#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "AllHeaders.hpp"

class Client
{
	int	_client_fd;

	public:
		
		Client();
		Client(Client const &copy);
		~Client();

		Client	&operator=(Client const &assign);

		void	setSocketFd(int fd);

		int		getSocketFd();
};

#endif