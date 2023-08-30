#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "AllHeaders.hpp"

class Webserv
{
	public:
		Webserv();
		// Webserv(Webserv const &copy);
		~Webserv();

		// Webserv	&operator=(Webserv const &assign);

		void		config(std::string conf);
		int			checkFile(std::string path);
		std::string	readContent(std::string path);
};

#endif