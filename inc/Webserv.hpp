/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjorus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 13:29:34 by yjorus            #+#    #+#             */
/*   Updated: 2023/10/24 13:29:36 by yjorus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		~Webserv();

		Webserv	&operator=(Webserv const &assign);

		void		config(std::string conf);
		std::string	readContent(std::string path);
		void		storeServerBlocks(std::string &content);
		size_t		findBlockStart(size_t a, std::string &content);
		size_t		findBlockEnd(size_t a, std::string &content);

		void	checkDuplicateServers();
		std::vector<Server>	getServers();
};

#endif
