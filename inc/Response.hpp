/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:49:46 by gscarama          #+#    #+#             */
/*   Updated: 2023/10/03 15:00:36 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
#include <map>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <sstream>

class Response
{
	private:
		Request		&_request;
		std::string	_error_msg; //From where it takes the number ?
		std::string	_header; //HTTP/1.1 200 OK
		std::string	_contentType;
		std::string	_conexion;
		size_t		_content_lenght;
		std::string	_server;
		std::string	_date;
		std::string	_body;
		short		_code;

	public:
		Response( void );
		Response( Request request );
		Response( Response const &other );
		Response& operator=( Response const &other );
		void	findErrorMsg();
		void	buildHeader();
		void	defineType();
		void	setConection();
		void	findLenght();
		void	setServer();
		void	setDate();
		void	buildBody();
};

#endif
