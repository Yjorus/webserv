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
		int			_method; // 0=GET 1=POST 2=DELETE
		int			_methodindex;
		int			_error_code;
		std::string	_location; // example /index.html
		std::string _query;
		std::string _fragment;
		std::string	_host; // example 127.0.0.1
		std::string	_useragent; // example curl
		std::string	_buffer;
		std::string	_body;
		ParseStep	_step;
		bool		_hasbody;
		bool		_ischunked;
		bool		_http_major;
		bool		_http_minor;
		std::string	_field_name_storage;
		size_t		_content_length;
		size_t		_length_of_chunk;

		std::map<int, std::string>	_methods;
		std::map<std::string, std::string> _headers;

	public:

		Request();
		~Request();
		void	parseRequest(std::string requeststr, size_t requestsize);
		bool	illegalCharLocation(int c);
		bool	checkScopePath(std::string path);
		bool	isIllegalToken(int c);
		void	addToHeaders(std::string &str1, std::string &str2);
		void	trimWhitespace(std::string &str);
		void	checkHeaders();

		int	getErrorCode();
};

#endif