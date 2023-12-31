/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjorus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 13:29:08 by yjorus            #+#    #+#             */
/*   Updated: 2023/10/24 13:29:09 by yjorus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "AllHeaders.hpp"

enum ParseStep
{
	start_parsing,
	check_method,
	check_first_space,
	check_slash_path,
	check_path,
	check_path_query,
	check_path_fragment,
	H,
	HT,
	HTT,
	HTTP,
	HTTP_slash,
	HTTP_major,
	HTTP_dot,
	HTTP_minor,
	first_carriage_return,
	first_new_line,
	determine_field,
	end_of_field,
	determine_field_name,
	determine_field_value,
	check_field_end,
	start_body,
	start_chunked,
	get_chunk_length,
	get_chunk_cr,
	get_chunk_nl,
	chunk_skip,
	get_chunk_data,
	get_chunk_cr2,
	get_chunk_nl2,
	end_of_chunk,
	end_of_chunk2,
	request_handled

};

class Request
{
		int			_method;
		size_t		_methodindex;
		int			_error_code;
		std::string	_location;
		std::string _query;
		std::string _fragment;
		std::string	_host;
		std::string _port;
		std::string	_address;
		std::string	_useragent;
		std::string	_buffer;
		std::string	_body;
		std::string	_boundary;
		ParseStep	_step;
		bool		_hasbody;
		bool		_ischunked;
		bool		_http_major;
		bool		_http_minor;
		bool		_multipart;
		std::string	_field_name_storage;
		size_t		_content_length;
		size_t		_length_of_chunk;

		std::map<int, std::string>	_methods;
		std::map<std::string, std::string> _headers;

	public:

		Request();
		~Request();
		Request(Request const &copy);
		Request	&operator=(Request const &assign);
		void	parseRequest(char *requeststr, size_t requestsize);
		bool	illegalCharLocation(int c);
		bool	checkScopePath(std::string path);
		bool	isIllegalToken(int c);
		void	addToHeaders(std::string &str1, std::string &str2);
		void	trimWhitespace(std::string &str);
		void	checkHeaders();
		void	setAddress(uint32_t address);
		void	setBody(std::string body);

		int									getErrorCode();
		std::string							getServerName();
		std::string							getAddress();
		std::string							getMethod();
		std::string							getQuery();
		std::map<std::string, std::string>	getHeaders();
		std::string							getHeader(std::string key);
		std::string							&getBody();
		std::string							getPort();
		std::string							getLocation();
		bool								keepAlive();
		bool								getMultiPart();
		std::string							getBoundary();

		void	clearRequest();
		
		bool	isFinished();
};

#endif
