#include "../inc/Request.hpp"

Request::Request()
{
	this->_methods[0] = "GET";
	this->_methods[1] = "POST";
	this->_methods[2] = "DELETE";
	this->_method = -1;
	this->_methodindex = 1;
	this->_error_code = 0;
	this->_location = "";
	this->_query = "";
	this->_fragment = "";
	this->_body = "";
	this->_hasbody = false;
	this->_ischunked = false;
	this->_step = start_parsing;
	this->_http_major = false;
	this->_http_minor = false;
	this->_field_name_storage = "";
	this->_content_length = 0;
	this->_length_of_chunk = 0;
}

Request::~Request()
{}


// "!" / "#" / "$" / "%" / "&" / "'" / "*"
//                  / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~"
//                  / DIGIT / ALPHA
//                  ; any VCHAR, except delimiters

bool	Request::isIllegalToken(int c)
{
	if (c == '!' || (c >= 35 && c <= 39) || (c >= 94 && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '*' || c == '+' || c == '-' || c == '.' || c == '|' || c == '~')
		return (false);
	return (true);
}



bool	Request::illegalCharLocation(int c)
{
	if ((c >= '?' && c <= '[') || (c >= 'a' && c <= 'z') || (c >= '#' && c <= ';') \
		|| c == '_' || c == ']' || c == '=' || c == '!' || c == '~')
		return (false);
	return (true);
}

bool	Request::checkScopePath(std::string path)
{
	std::string	hold(path);
	char	*pch;
	pch = strtok((char *)hold.c_str(),"/");
	int scope = 0;
	while (pch != NULL)
	{
		if (!strcmp(pch, ".."))
			scope--;
		else if (strcmp(pch, "."))
			scope++;
		if (scope < 0)
			return (true);
		pch = strtok (NULL, "/");
	}
	return (false);
}

void	Request::trimWhitespace(std::string &str)
{
	const char *ws = " \t";
	str.erase(0, str.find_first_not_of(ws));
	str.erase(str.find_last_not_of(ws) + 1);
}

void	Request::addToHeaders(std::string &str1, std::string &str2)
{
	trimWhitespace(str1);
	trimWhitespace(str2);
	std::cout << str1 << ": " << str2 << std::endl; 
	this->_headers[str1] = str2;
}

void	Request::checkHeaders()
{
	if (_headers.count("content-length"))
	{
		_hasbody = true;
		std::stringstream ss;
		ss << _headers["content-length"];
		ss >> _content_length;
	}
	if (_headers.count("transfer-encoding"))
	{
		_hasbody = true;
		if (_headers["transfer-encoding"].find_first_of("chunked") != std::string::npos)
			_ischunked = true;
	}
	if (_headers.count("host"))
		_host = _headers["host"];
}

void	Request::parseRequest(std::string requeststr, size_t requestsize)
{
	int					parsechar;
	std::stringstream	ss;

	for (std::string::size_type a = 0; a < requestsize; a++) {
		parsechar = requeststr[a];
		// std::cout << (char)parsechar << std::endl;
		switch(_step) {
			case start_parsing: {
				if (parsechar == 'G')
					_method = 0;
				else if (parsechar == 'P')
					_method = 1;
				else if (parsechar == 'D')
					_method = 2;
				else {
					_error_code = 501;
					return ;
				}
				_step = check_method;
				continue;
			}
			case check_method: {
				if (parsechar == _methods[_method][_methodindex])
					_methodindex++;
				else {
					_error_code = 501;
					return ;
				}
				if (_methodindex == _methods[_method].length()) {
					std::cout << "method: " << _methods[_method] << std::endl;
					_step = check_first_space;
				}
				continue;
			}
			case check_first_space: {
				if (parsechar != ' ') {
					_error_code = 400;
					return;
				}
				_step = check_slash_path;
				continue;
			}
			case check_slash_path: {
				if (parsechar != '/') {
					_error_code = 400;
					return ;
				}
				_step = check_path;
				break;
			}
			case check_path: {
				if (parsechar == ' ') {
					_step = H;
					_location.append(_buffer);
					std::cout << "path: " << _buffer << std::endl;
					_buffer.clear();
					continue;
				}
				else if (parsechar == '?') {
					_step = check_path_query;
					_location.append(_buffer);
					std::cout << "path: " << _buffer << std::endl;
					_buffer.clear();
					continue;
				}
				else if (parsechar == '#') {
					_step = check_path_fragment;
					_location.append(_buffer);
					std::cout << "path: " << _buffer << std::endl;
					_buffer.clear();
					continue;
				}
				else if (illegalCharLocation(parsechar)) {
					_error_code = 400;
					return;
				}
				else if (a > 2048) {
					_error_code = 414;
					return;
				}
				break;
			}
			case check_path_query: {
				if (parsechar == ' ') {
					_step = H;
					_query.append(_buffer);
					std::cout << "query: " << _buffer << std::endl;
					_buffer.clear();
					continue;
				}
				else if (parsechar == '#') {
					_step = check_path_fragment;
					_query.append(_buffer);
					std::cout << "query: " << _buffer << std::endl;
					_buffer.clear();
					continue;
				}
				else if (illegalCharLocation(parsechar)) {
					_error_code = 400;
					return;
				}
				else if (a > 2048) {
					_error_code = 414;
					return;
				}
				break;
			}
			case check_path_fragment: {
				if (parsechar == ' ') {
					_step = H;
					_fragment.append(_buffer);
					std::cout << "fragment: " << _buffer << std::endl;
					_buffer.clear();
					continue;
				}
				else if (illegalCharLocation(parsechar)) {
					_error_code = 400;
					return;
				}
				else if (a > 2048) {
					_error_code = 414;
					return;
				}
				break;
			}
			case H: {
				if (checkScopePath(this->_location) || parsechar != 'H') {
					_error_code = 400;
					return ;
				}
				_step = HT;
				break ;
			}
			case HT: {
				if (parsechar != 'T') {
					_error_code = 400;
					return ;
				}
				_step = HTT;
				break;
			}
			case HTT: {
				if (parsechar != 'T') {
					_error_code = 400;
					return ;
				}
				_step = HTTP;
				break;
			}
			case HTTP: {
				if (parsechar != 'P') {
					_error_code = 400;
					return ;
				}
				_step = HTTP_slash;
				break;
			}
			case HTTP_slash: {
				if (parsechar != '/') {
					_error_code = 400;
					return ;
				}
				_step = HTTP_major;
				break ;
			}
			case HTTP_major: {
				if (parsechar != '1') {
					_error_code = 400;
					return;
				}
				_http_major = true;
				_step = HTTP_dot;
				break ;
			}
			case HTTP_dot: {
				if (parsechar != '.') {
					_error_code = 400;
					return ;
				}
				_step = HTTP_minor;
				break ;
			}
			case HTTP_minor: {
				if (parsechar != '1') {
					_error_code = 400;
					return ;
				}
				_http_minor = true;
				_step = first_carriage_return;
				break ;
			}
			case first_carriage_return: {
				if(parsechar != '\r') {
					_error_code = 400;
					return;
				}
				_step = first_new_line;
				break ;
			}
			case first_new_line: {
				if (parsechar != '\n') {
					_error_code = 400;
					return ;
				}
				_step = determine_field;
				_buffer.clear();
				continue ;
			}
			case determine_field: {
				if (parsechar == '\r')
					_step = end_of_field;
				else if (!isIllegalToken(parsechar))
					_step = determine_field_name;
				else {
					_error_code = 400;
					return ;
				}
				break ;
			}
			case end_of_field: {
				if (parsechar != '\n') {
					_error_code = 400;
					return ;
				}
				else {
					_buffer.clear();
					checkHeaders();
					if (_hasbody == true) {
						if (_ischunked == true)
							_step = start_chunked;
						else
							_step = start_body;
					}
					else {
						_step = request_handled;
						continue ;
					}
				}
				break;
			}
			case determine_field_name: {
				if (parsechar == ':') {
					_field_name_storage = _buffer;
					_buffer.clear();
					_step = determine_field_value;
					continue ;
				}
				else if (isIllegalToken(parsechar)) {
					_error_code = 400;
					return ;
				}
				break ;
			}
			case determine_field_value: {
				if (parsechar == '\r') {
					addToHeaders(_field_name_storage, _buffer);
					_field_name_storage.clear();
					_buffer.clear();
					_step = check_field_end;
					continue ;
				}
				break ;
			}
			case check_field_end: {
				if (parsechar != '\n') {
					_error_code = 400;
					return ;
				}
				_step = determine_field;
				continue ;
			}
			case start_body: {
				if (_body.size() < _content_length)
					_body.push_back(parsechar);
				if (_body.size() == _content_length)
					_step = request_handled;
				break;
			}
			case start_chunked: {
				if (!isxdigit(parsechar)) {
					_error_code = 400;
					return ;
				}
				ss.str(std::string());
				ss.clear();
				ss << parsechar;
				ss >> std::hex >> _length_of_chunk;
				if (_length_of_chunk)
					_step = get_chunk_length;
				else
					_step = get_chunk_cr;
				continue;
			}
			case get_chunk_length: {
				if (isxdigit(parsechar)) {
					size_t	lmao = 0;
					ss.str(std::string());
					ss.clear();
					ss << parsechar;
					ss >> std::hex >> lmao;
					_length_of_chunk *= 16;
					_length_of_chunk += lmao;
				}
				else if (parsechar == '\r')
					_step = get_chunk_nl;
				else
					_step = chunk_skip;
				continue ;
			}
			case chunk_skip: {
				if (parsechar == '\r')
					_step = get_chunk_nl;
				continue ;
			}
			case get_chunk_cr: {
				if (parsechar != '\r') {
					_error_code = 400;
					return ;
				}
				_step = get_chunk_nl;
				continue ;
			}
			case get_chunk_nl: {
				if (parsechar != '\n') {
					_error_code = 400;
					return ;
				}
				if (_length_of_chunk == 0)
					_step = end_of_chunk;
				else
					_step = get_chunk_data;
				continue ;
			}
			case get_chunk_data: {
				_body.push_back(parsechar);
				_length_of_chunk--;
				if (_length_of_chunk == 0)
					_step = get_chunk_cr2;
				continue ;
			}
			case get_chunk_cr2: {
				if (parsechar != '\r') {
					_error_code = 400;
					return ;
				}
				_step = get_chunk_nl2;
				continue ;
			}
			case get_chunk_nl2: {
				if (parsechar != '\n') {
					_error_code = 400;
					return ;
				}
				_step = start_chunked;
				continue ;
			}
			case end_of_chunk: {
				if (parsechar != '\r') {
					_error_code = 400;
					return ;
				}
				_step = end_of_chunk2;
				continue ;
			}
			case end_of_chunk2: {
				if (parsechar != '\n') {
					_error_code = 400;
					return ;
				}
				_step = request_handled;
			}
			case request_handled: {
				return ;
			}
		}
		_buffer += parsechar;
	}
}

int	Request::getErrorCode()
{
	return (this->_error_code);
}
