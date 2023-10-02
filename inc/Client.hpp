#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "AllHeaders.hpp"
# include "Request.hpp"

class Client
{
	int			_client_fd;
	std::string	_request_str;
	Request		_request;

	public:
		
		Client();
		// Client(Client const &copy);
		~Client();

		// Client	&operator=(Client const &assign);

		void	setSocketFd(int fd);
		void	updateRequest(std::string request);

		int		getSocketFd();
		std::string	getRequestStr();
		Request		&getRequest();
};

#endif