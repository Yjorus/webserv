#include "../inc/Request.hpp"

Request::Request()
{
	this->_methods[0] = "GET";
	this->_methods[1] = "POST";
	this->_methods[2] = "DELETE";
	this->_method = -1;
	this->_error_code = 0;
	this->_location = "";
	this->_query = "";
	this->_fragment = "";
	this->_hasbody = false;
	this->_step = start_parsing;
}

Request::~Request()
{}

void	Request::parseRequest(std::string requeststr, size_t request_size)
{
	int	parsechar;
	for (std::string::size_type a = 0; a < requeststr.size(); a++)
	{
		parsechar = requeststr[a];
		swich(_step)
		{
			case start_parsing:
			{
				if (parsechar == 'G')
					_method = 0;
				else if (parsechar == 'P')
					_method = 1;
				else if (parsechar == 'D')
					_method = 2;
				else
				{
					_error_code = 501;
					return ;
				}
				_step = check_method;
				continue;
			}
			case check_method:
			{
				if (parsechar == _methods[_method][_methodindex])
					_methodindex++;
				else
				{
					_error_code = 501;
					return ;
				}
				if (_methodindex == _methods[_method].length())
					_step = first_space;
				continue;
			}
			case check_first_space:
			{
				if (parsechar != ' ')
				{
					_error_code = 400;
					return;
				}
				_step = check_slash_path;
				continue;
			}
			case check_slash_path:
			{
				if (parsechar != '/')
				{
					_error_code = 400;
					return ;
				}
				_step = check_path;
				break;
			}
			case check_path:
			{
				if (parsechar == ' ')
				{
					_step = http_version;
					_location.append(_buffer);
					_buffer.clear();
					continue;
				}
				else if (parsechar == '?')
				{
					_step = check_path_query;
					_location.append(_buffer);
					_buffer.clear();
					continue;
				}
				else if (parsechar == '#')
				{
					_step = check_path_fragment;
					_location.append(_buffer);
					_buffer.clear();
					continue;
				}
				else if (legalCharLocation(parsechar))
				{
					_error_code = 400;
					return;
				}
				else if (a > 2048)
				{
					_error_code = 414;
					return;
				}
				break;
			}
			case check_path_query:
			{
				if (parsechar == ' ')
				{
					_step = http_version;
					_query.append(_buffer);
					_buffer.clear();
					continue;
				}
				else if (parsechar == '#')
				{
					_step = check_path_fragment;
					_query.append(_buffer);
					_buffer.clear();
					continue;
				}
				else if (legalCharLocation(parsechar))
				{
					_error_code = 400;
					return;
				}
				else if (a > 2048)
				{
					_error_code = 414;
					return;
				}
				break;
			}
			case check_path_fragment:
			{
				if (parsechar == ' ')
				{
					_step = http_version;
					_fragment.append(_buffer);
					_buffer.clear();
					continue;
				}
				else if (legalCharLocation(parsechar))
				{
					_error_code = 400;
					return;
				}
				else if (a > 2048)
				{
					_error_code = 414;
					return;
				}
				break;
			}
			case http_version:
			{}
		}
		_buffer += parsechar;
	}
}
