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
	http_version

}

class Request
{
		int	_method; // 1=GET 2=POST 3=DELETE
		std::map<int, std::str>	_methods;
		int	_error_code;
		std::string	_location; // example /index.html
		std::string _query;
		std::string _fragment;
		std::string	_host; // example 127.0.0.1
		std::string	_useragent; // example curl
		std::string	_buffer;
		bool _hasbody;
		std::string	_body;
		ParseStep	_step;

	public:

		Request();
		~Request();
		void	parseRequest(std::string requeststr, size_t requestsize);
};

#endif