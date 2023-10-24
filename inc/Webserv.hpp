#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "AllHeaders.hpp"
# include "Server.hpp"

class Webserv
{
		int	_server_amount;
		std::vector<std::string> _server_blocks;
		std::vector<Server>	_servers;
		std::map<int, int>		_connections;
	public:
		Webserv();
		Webserv(Webserv const &copy);
		Webserv	&operator=(Webserv const &other);
		~Webserv();

		void		config(std::string conf);
		std::string	readContent(std::string path);
		void		storeServerBlocks(std::string &content);
		size_t		findBlockStart(size_t a, std::string &content);
		size_t		findBlockEnd(size_t a, std::string &content);

		void	checkDuplicateServers();
		std::vector<Server>	getServers();
};

#endif