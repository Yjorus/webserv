/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:51:12 by gscarama          #+#    #+#             */
/*   Updated: 2023/10/10 12:54:57 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

Response::Response( void )
{
	this->_host = "";
	this->_code = 0;
	this->_status_msg = "";
	this->_header = "";
	this->_contentType = "";
	this->_conexion = "";
	this->_date = "";
	this->_body = "";
	this->_content_lenght = "";
	this->_root = "";
	this->_code = 0;
}

Response::Response( Response const &other ) : _request(other._request)
{
	*this = other;
}

Response& Response::operator=( Response const &other )
{
	if (this != &other)
	{
		this->_request = other._request;
		this->_status_msg = other._status_msg;
		this->_header = other._header;
		this->_contentType = other._contentType;
		this->_conexion = other._conexion;
		this->_content_lenght = other._content_lenght;
		this->_server = other._server;
		this->_date = other._date;
		this->_body = other._body;
	}
	return (*this);
}

void	Response::initializeResponse( Request &request, Server server)
{
	this->_code = 0;
	this->_request = request;
	this->_server = server;
	this->_error_pages = server.getErrorPages();
	this->_root = "./" + server.getRoot();
}

void	Response::findStatusMsg()
{
	if (this->_request.getErrorCode() != 0)
		this->_code = _request.getErrorCode();
	this->_status_msg = statusCodes(this->_code);
}

void	Response::buildHeader()
{
	if (_code == 0)
		_code = 200;
	this->_header = "HTTP/1.1 ";
	_header.append(to_String(_code));
	this->_header.append(" ");
	this->_header.append(statusCodes(_code));
	this->_header.append("\r\n");
}

void	Response::defineType()
{
	std::string		ext;
	int				i;

	ext = _request.getLocation();
	i = ext.find('.');
	ext.erase(0, i);
	this->_contentType.append("Content-Type: ");
	if (ext == ".html" || ext == ".htm")
		this->_contentType = "text/html; charset=UTF-8";
	else if (ext == ".css")
		this->_contentType.append("text/css");
	else if (ext == ".js")
		this->_contentType.append("text/javascript");
	else if (ext == ".jpeg")
		this->_contentType.append("image/jpeg");
	else if (ext == ".png")
		this->_contentType.append("image/png");
	else if (ext == ".ico")
		this->_contentType.append("image/x-icon");
	else 
		this->_contentType.append("text/html");
	this->_contentType.append("\r\n");
}

void	Response::setConnection()
{
	this->_conexion = "Connection: keep-alive\r\n";
}

void	Response::findLenght()
{
	this->_content_lenght = "Content-Length: ";
	this->_content_lenght.append(to_String(this->_body.length()));
	this->_content_lenght.append("\r\n");
}

void	Response::setServer()
{
	if (_request.getServerName() != "")
		this->_host = _request.getServerName();
	else
		this->_host = "LulzSec Server";
	this->_host.append("\r\n");
}

void	Response::setDate()
{
	time_t		now = time(0);
	std::string	utc;

	tm* gmtm = gmtime(&now);
	if (gmtm != NULL)
		utc = asctime(gmtm);
	else
	{
		this->_code = 501;
		return ;
	}
	if (utc[8] == ' ')
		utc.replace(8, 1, "0");
	utc.insert(3,",");
	utc.insert(8, &utc[20]);
	utc.erase(13, 1);
	utc.erase(25, 6);
	utc.append(" UTC");
	utc.append("\r\n");
	this->_date = "Date: ";
	this->_date.append(utc);
}

void	Response::buildBody()
{
	std::ifstream		file;

	if (_request.getLocation() == "/")
		file.open(_root + "index.html");
	else
		file.open(_root + _request.getLocation());
	if(file.fail())
	{
		file.open(_root + "error/404.html");
		this->_code = 404;
	}
	this->_body = readFile(file);
}

void	Response::buildErrorBody()
{
	std::ifstream		file;

	if (this->_error_pages.count(this->_code))
		file.open(_root + this->_error_pages[this->_code]);
	else if (checkFile(_root + "error/" + to_String(_code) + ".html"))
		file.open(_root + "error/" + to_String(_code) + ".html");
	if (!file.good())
	{
		file.open(_root + "error/502.html");
		this->_code = 502;
	}
	this->_body = readFile(file);
}

void	Response::buildResponse()
{
	if (_request.getMethod() == "POST") //Execut the CGI
		std::cout << "BODY: " <<  _request.getBody() << std::endl;
	this->defineType();
	this->setDate();
	this->setConnection();
	if(_request.getErrorCode() != 0 || this->_code != 0)
		this->buildErrorBody();
	else
		this->buildBody();
	this->findLenght();
	this->findStatusMsg();
	this->buildHeader();

	this->_header.append(this->_conexion);
	this->_header.append(this->_contentType);
	this->_header.append(this->_date);
	this->_header.append(this->_content_lenght);
	this->_header.append("\r\n");
	this->_header.append(this->_body);
}

std::string	Response::getResponse()
{
	return(this->_header);
}

void	Response::cutResponse(size_t a) {
	this->_header = this->_header.substr(a);
}

void	Response::clearResponse()
{
	this->_code = 0;
	this->_host.clear();
	this->_status_msg.clear();
	this->_header.clear();
	this->_contentType.clear();
	this->_conexion.clear();
	this->_date.clear();
	this->_body.clear();
	this->_content_lenght.clear();
}
