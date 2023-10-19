/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:49:46 by gscarama          #+#    #+#             */
/*   Updated: 2023/10/19 14:33:25 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "AllHeaders.hpp"
# include "Request.hpp"
# include "Server.hpp"
# include "CgiManager.hpp"

class Response
{
		// void		findStatusMsg();
		void		buildHeader();
		void		defineType();
		void		setConnection();
		void		findLenght();
		void		setServer();
		void		setDate();
		bool		buildBody();
		void		buildErrorBody();
	public:
	
		Response( void );
		Response( Response const &other );
		Response& operator=( Response const &other );
		void		initializeResponse( Request &request);
		void		buildResponse();
		void		clearResponse();
		std::string	getResponse();
		void		cutResponse(size_t a);
		void		updateResponse(char *buffer, int a);

		void	getLocationPath(std::string path, std::vector<Location> locations, std::string &locationpath);
		bool	checkMethod(std::string method, std::vector<bool> allowed);
		bool	checkRedirection(Location location);
		std::string combinePaths(std::string str1, std::string str2, std::string str3);
		void	replaceProxy(Location location);
		void	combineRootPath(Location location);
		bool 	isDir(std::string path);
		bool 	realFile (const std::string& f);
		bool	checkLocation();
		bool	checkCgi(std::string &locationpath);
		bool	checkErrorCode();
		int		buildDirectoryListing(std::string path, std::string &listingbody);
		std::string	handleBoundary(std::string content, std::string boundary);

		void	setCgiErrorResponse(int a);

		void	setServer(Server &server);
		Server	getServer();

		void		setCgiFlag(int a);
		int			getCgiFlag();

		CgiManager	&getCgiManager();

	private:
		Server						_server;
		Request						_request;
		CgiManager					_cgi_manager;
		int							_cgi_flag;
		int							_cgi_fd[2];
		std::map<int, std::string>	_error_pages;
		std::string					_status_msg;
		std::string					_header;
		std::string					_contentType;
		std::string					_conexion;
		std::string					_host;
		std::string					_date;
		std::string					_body;
		std::string					_content_lenght;
		std::string					_location;
		std::string					_full_path;
		std::string					_listingbody;
		int							_code;
		bool						_listing;
};

#endif
