#ifndef SERVER_HPP
# define SERVER_HPP

# include "AllHeaders.hpp"
# include "Location.hpp"

class Server
{
		unsigned long				_client_body_size;
		std::string					_host;
		std::string					_port;
		std::string					_index;
		std::string					_server_name;
		std::string					_root;
		std::map<int, std::string>	_error_pages;
		bool						_directory_listing;
		std::vector<Location>		_locations;


	public:
		
		Server();
		Server(Server const &copy);
		~Server();

		Server	&operator=(Server const &assign);
		
		void						config(std::string config);
		std::vector<std::string>	configSplit(std::string config, std::string separators);
		void						setHost(std::string ip);
		void						setPort(std::string port);
		void						setClientBodySize(std::string size);
		void						setIndex(std::string index);
		void						setServerName(std::string name);
		void						setRoot(std::string root);
		void						setDirectoryListing(std::string listing);
		void						setErrorPages(std::vector<std::string> pages);
		void						setLocation(std::string path, std::vector<std::string> data);

		bool	checkHost(std::string ip);
		void	checkSemicolon(std::string &str);
		int		locationCheck(Location &location)const;
		int		checkDuplicateLocationPaths(void)const;

		std::string					getPort()const;
		std::string					getHost()const;
		unsigned long				getClientBodySize()const;
		std::string					getIndex()const;
		std::string					getServerName()const;
		std::string					getRoot()const;
		std::map<int, std::string>	getErrorPages()const;
		bool						getDirectoryListing()const;

};

std::ostream	&operator<<(std::ostream &o, Server const &server);

#endif