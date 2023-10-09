/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:51:12 by gscarama          #+#    #+#             */
/*   Updated: 2023/10/03 15:45:07 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

Response::Response( void ) : _request(Request req())
{
	 // does it need a get element ?
	// this->_request();

	//Return a error ? OR just set all to null, send a response error?
	throw std::invalid_argument("Error: Response class not initialized");
}

Response::Response( Request request ) : _request(request)
{
	// Check if theres a error in request
	if(_request._code != 0)
		
	this->defineType();
	this->setConection();
	this->setDate();
	this->setConection();
	this->findLenght();
	//Just set some stuff, majory of it will be seted in build body
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
		this->_error_msg = other._error_msg;
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

void	Response::findErrorMsg() //receive the errors page and try to find witch ine matchs
{
	//Try to find it inside map, if not found set default page
	//Pointe to the resault saving memory
	//If no one matcher send 502 Bad Gatey as ettpt
}

void	Response::buildHeader()
{
	std::stringstream	code;
	std::string			tmp;

	
	this->_header = "HTTP/1.1 ";
	code << _code;
	code >> tmp;
	_header.append(tmp);
	this->_header.append(" ");
	this->_header += this->_error_msg;
	this->_header.append("\n\0"); // Is the final correct ?
}

void	Response::defineType()
{
	std::string		ext;
	std::ifstream	file;

	if (file.open(_request._location))//Does file exist ?
	{
		this->_code = 404;
		this->_contentType = "text/html";
	}
	else
	{
		ext = _request._location.find('.');
		if (ext == ".html" || ext == "htm")
			this->_contentType = "text/html"; //charset=UTF-8 ??
		else if (ext == ".css")
			this->_contentType = "text/css";
		else if (ext == ".js")
			this->_contentType = "text/html";
		else if (ext == ".jpeg")
			this->_contentType = "image/jpeg";
		else if (ext == ".png")
			this->_contentType = "image/png";
		else if (ext == ".ico")
			this->_contentType = "image/x-icon"; //Check for more possibble extentions
		else
			this->_contentType = "text/html";
		this->_code = 200; //What is the possile codes i can return here ?
	}

	//Html/css/img/script
}

void	Response::setConection()
{
	// what is the diferent types of connection ?
	// keep-alive
	this->_conexion = "keep-alive";
}

void	Response::findLenght()
{
	size_t	lenght;

	// lenght = response file size
	//Calculate the lenght from the content
	//add lenght to body
	this->_content_lenght = lenght;
}

void	Response::setServer()
{
	
	//How to get the server name ?
}

void	Response::setDate()
{
	time_t now = time(0);

	tm* gmtm = gmtime(&now);
	if (gmtm != NULL)
		this->_date = asctime(gmtm);
	else
	{
		this->_code = 501; // Correct code ?
		return ;
	}
	if (this->_date[8] == ' ')
		this->_date.replace(8, 1,"0");
	this->_date.insert(3,",");
	this->_date.insert(8, &this->_date[20]);
	this->_date.erase(13, 1);
	this->_date.erase(25, 6);
	this->_date.append(" UTC"); //Need to a add a /n/0 in the end ?
}

void	Response::buildBody()
{
	//Initialize all the variables

	this->findErrorMsg();
	this->buildHeader();
	//Error found inside the response
	//Outise the response
	//error body

	//Build response body.
		//Open asked file
		//read thhe file
		//send the file in the body

	//Send this to where

	//Check if error page exist, if dont return a 502 Error Bad Gatway // TODO Where
}
