/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:49:46 by gscarama          #+#    #+#             */
/*   Updated: 2023/10/09 15:24:44 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "AllHeaders.hpp"
#include "Request.hpp"

class Response
{
	private:
		Request						_request;
		std::map<int, std::string>	_error_pages;
		std::string					_status_msg;
		std::string					_header;
		std::string					_contentType;
		std::string					_conexion;
		std::string					_server;
		std::string					_date;
		std::string					_body;
		std::string					_content_lenght;
		int							_code;

	public:
		Response( void );
		Response( Request request, std::map<int, std::string> error_pages);
		Response( Response const &other );
		Response& operator=( Response const &other );
		void		findStatusMsg();
		void		buildHeader();
		void		defineType();
		void		setConnection();
		void		findLenght();
		void		setServer();
		void		setDate();
		void		buildBody();
		void		buildErrorBody();
		const char	*buildResponse();
		void		clearResponse();
};

#endif
