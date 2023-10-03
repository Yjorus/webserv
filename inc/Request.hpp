
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

class Request
{
		int	_Method; // 1=GET 2=POST 3=DELETE
		std::string	_location; // example /index.html
		std::string	_host; // example 127.0.0.1
		std::string	_useragent; // example curl
		std::string	_body;
	public:
		Request();
};

#endif
