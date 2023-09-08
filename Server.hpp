#ifndef SERVER_HPP
# define SERVER_HPP

# include "Allheaders.hpp"

class Server
{
		unsigned long				_client_body_size;
		std::string					_host;
		std::string					_port;
		std::string					_index;
		std::string					_server_name;
		std::string					_root;
		std::map<int, std::string>	_error_codes;
		bool						_directory_listing;
		std::vector<Location>		_locations;


	public:
		
		Server();
		Server(Server const &copy);
		~Server();

		Server	&operator=(Server const &assign);
		
		void	config(std::string config);
		void	setHost(std::string ip);
		void	setPort(std::string port);
		void	setClientBodySize(std::string size);
		void	setIndex(std::string index);
		void	setServerName(std::string name);
		void	setRoot(std::string root);
		void	setDirectoryListing(std::string listing);
		void	setErrorPages(); // not sure yet

		bool	checkHost(std::string ip);
		void	checkSemicolon(std::string &str);

		int							getPort()const;
		uint32_t					getHost()const;
		unsigned long				getClientBodySize()const;
		std::string					getIndex()const;
		std::string					getServerName()const;
		std::string					getRoot()const;
		std::map<int, std::string>	getErrorPages()const;
		bool						getDirectoryListing()const;
};

#endif