/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:51:12 by gscarama          #+#    #+#             */
/*   Updated: 2023/10/09 15:36:04 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

Response::Response( void )
{
	this->_server = "";
	this->_code = 0;
	this->_status_msg = "";
	this->_header = "";
	this->_contentType = "";
	this->_conexion = "";
	this->_date = "";
	this->_body = "";
	this->_content_lenght = "";
	this->_code = 0;
}

Response::Response( Response const &other ) : _request(other._request)
{
	*this = other;
}

Response& Response::operator=( Response const &other )
{
	if (this != &other) //Add geter for all this ?
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

void	Response::initializeResponse( Request request, std::map<int, std::string> error_pages)
{
	this->_request = request;
	this->_error_pages = error_pages;
}

void	Response::findStatusMsg()
{
	if (this->_request.getErrorCode() != 0)
		this->_status_msg = statusCodes(_request.getErrorCode());
	else
		this->_status_msg = statusCodes(this->_code);
}

void	Response::buildHeader()
{
	this->_header = "HTTP/1.1 ";
	_header.append(to_String(_code));
	this->_header.append(" ");
	this->_header += statusCodes(_code);
	this->_header.append("\r\n");
}

void	Response::defineType() // add contetnt type
{
	std::string		ext;
	std::ifstream	file;

	file.open(_request.getLocation());
	this->_contentType.append("Content-Type: ");
	if (file.fail())
	{
		this->_code = 404;
		this->_contentType = "text/html";
		return ;
	}
	else
	{
		ext = _request.getLocation().find('.');
		if (ext == ".html" || ext == "htm")
			this->_contentType = "text/html;charset=UTF - 8"; //Add -utf-08
		else if (ext == ".css")
			this->_contentType.append("text/css");
		else if (ext == ".js")
			this->_contentType.append("text/html"); // ??????????
		else if (ext == ".jpeg")
			this->_contentType.append("image/jpeg");
		else if (ext == ".png")
			this->_contentType.append("image/png");
		else if (ext == ".ico")
			this->_contentType.append("image/x-icon");
		else
			this->_contentType.append("text/html");
	}
	this->_contentType.append("\r\n");
}

void	Response::setConnection()
{
	this->_conexion = "Connection: keep-alive\r\n";
}

void	Response::findLenght()
{
	this->_content_lenght = "Content-Lenght: ";
	this->_content_lenght.append(to_String(this->_body.length()));
	this->_content_lenght.append("\r\n");
}

void	Response::setServer()
{
	if (_request.getServerName() != "")
		this->_server = _request.getServerName();
	else
		this->_server = "WebServ";
	this->_server.append("\r\n");
}

void	Response::setDate()
{
	time_t now = time(0);
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
	//Error found inside the response
	//Outise the response
	//error body

	//Build response body.
		//Open asked file
		//read thhe file
		//send the file in the body

	//Send this to where
	// how to send
	std::ifstream file("./rootdir/index.html");
	std::ostringstream		str;
	
	this->_code = 200;
	str << file.rdbuf();
	_body = str.str();
	file.close();
	// /r/n in the end of header content
	//Check if error page exist, if dont return a 502 Error Bad Gatway // TODO Where
}

void	Response::buildErrorBody()
{
	std::fstream	file;
	std::string		str;

	if (this->_error_pages.count(this->_code))
		file.open("./rootdir/" + this->_error_pages[this->_code]); //Check if i do it correctly
	else if (checkFile("./rootdir/error/" + to_String(_code) + ".html"))
		file.open("./rootdir/error/" + to_String(_code) + ".html");
	else
	{
		file.open("./rootdir/error/502.html");
		this->_code = 502;
	}
	while (!file.eof())
	{
		file >> str;
		this->_body.append(str);
	}
}

const char	*Response::buildResponse()
{
	this->defineType();
	this->setDate();
	this->setConnection();
	// if(_request.getErrorCode() != 0 || this->_code != 0)
	// 	this->buildErrorBody();
	// else
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
	
	std::cout << this->_header << std::endl;
	return(this->_header.c_str());
}

void	Response::clearResponse()
{
	this->_server = "";
	this->_code = 0;
	this->_status_msg = "";
	this->_header = "";
	this->_contentType = "";
	this->_conexion = "";
	this->_date = "";
	this->_body = "";
	this->_content_lenght = "";
	this->_code = 0;
}
