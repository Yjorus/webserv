/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:51:12 by gscarama          #+#    #+#             */
/*   Updated: 2023/10/23mmit 15:20:12 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

Response::Response( void )
{
	this->_host = "";
	this->_code = 0;
	this->_cgi_flag = 0;
	this->_status_msg = "";
	this->_header = "";
	this->_contentType = "";
	this->_conexion = "";
	this->_date = "";
	this->_body = "";
	this->_content_lenght = "";
	this->_listing = false;
	this->_full_path = "";
	this->_listingbody = "";
	this->_location = "";
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
		this->_code = other._code;
		this->_cgi_flag = other._cgi_flag;
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

void	Response::initializeResponse( Request &request)
{
	this->_code = 0;
	this->_cgi_flag = 0;
	this->_request = request;
	this->_listing = false;
	this->_error_pages = _server.getErrorPages();
}

void	Response::buildHeader()
{
	this->_header = "HTTP/1.1 ";
	_header.append(to_String(this->_code));
	this->_header.append(" ");
	this->_header.append(statusCodes(this->_code));
	this->_header.append("\r\n");
}

void	Response::defineType()
{
	std::string		ext;
	int				i;

	ext = this->_full_path;
	i = ext.find('.');
	ext.erase(0, i);
	this->_contentType.append("Content-Type: ");
	if (ext == ".html" || ext == ".htm")
		this->_contentType.append("text/html; charset=UTF-8");
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
		this->_contentType.append("text/plain");
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

	struct tm* gmtm = gmtime(&now);
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

void	Response::getLocationPath(std::string path, std::vector<Location> locations, std::string &locationpath) {
	size_t a = 0;

	for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
		if (path.find(it->getPathL()) == 0){
			if (it->getPathL() == "/" || path.length() == it->getPathL().length() || path[it->getPathL().length() - 1] == '/') {
				if (it->getPathL().length() > a) {
					a = it->getPathL().length();
					locationpath = it->getPathL();
				}
			}
		}
	}
}

bool	Response::checkMethod(std::string method, std::vector<bool> allowed) {
	if ((method == "GET" && allowed[0] != true) || (method == "POST" && allowed[1] != true) || (method == "DELETE" && allowed[2] != true)) {
		this->_code = 405;
		return (1);
	}
	return (0);
}

bool	Response::checkRedirection(Location location) {
	if (!location.getRedirectionL().empty()) {
		this->_code = 301;
		this->_location = location.getRedirectionL();
		if (this->_location[0] != '/')
			this->_location.insert(this->_location.begin(), '/');
		return (1);
	}
	return (0);
}

std::string Response::combinePaths(std::string str1, std::string str2, std::string str3) {
    std::string	combined;
    size_t		len1;
    size_t		len2;

    len1 = str1.length();
    len2 = str2.length();
    if (str1[len1 - 1] == '/' && (!str2.empty() && str2[0] == '/') )
        str2.erase(0, 1);
    if (str1[len1 - 1] != '/' && (!str2.empty() && str2[0] != '/'))
        str1.insert(str1.end(), '/');
    if (str2[len2 - 1] == '/' && (!str3.empty() && str3[0] == '/') )
        str3.erase(0, 1);
    if (str2[len2 - 1] != '/' && (!str3.empty() && str3[0] != '/'))
        str2.insert(str1.end(), '/');
    combined = str1 + str2 + str3;
    return (combined);
}

void	Response::replaceProxy(Location location) {
	this->_full_path = combinePaths(location.getProxyL(), this->_request.getLocation().substr(location.getPathL().length()), "");
	this->_full_path = combinePaths(location.getRootL(), this->_full_path, "");
}

void	Response::combineRootPath(Location location) {
	this->_full_path = combinePaths(location.getRootL(), this->_request.getLocation(), "");
}

bool Response::isDir(std::string path) {
    struct stat file_stat;
    if (stat(path.c_str(), &file_stat) != 0)
        return (false);

    return (S_ISDIR(file_stat.st_mode));
}

bool Response::realFile (const std::string& f) {
    std::ifstream file(f.c_str());
    return (file.good());
}

bool	Response::checkCgi(std::string &locationpath) {
	size_t a;
	std::string path = this->_full_path;

	if (path[path.size() - 1] == '/')
		path += this->_server.getLocationByPath(locationpath)->getIndexL();
	a = path.rfind('.');
	if (a == std::string::npos) {
		this->_code = 406;
		return (1);
	}
	std::string	extension = path.substr(a);
	if (extension != ".pl" && extension != ".py") {
		this->_code = 501;
		return (1);
	}
	if (checkFile(path) != 1 || checkPath(path, 1) == -1 || checkPath(path, 3) == -1) {
		this->_code = 404;
		return (1);
	}
	if (checkMethod(this->_request.getMethod(), this->_server.getLocationByPath(locationpath)->getMethodsL())) {
		this->_cgi_flag = 0;
		return (1);
	}
	this->_cgi_manager.clearCgi();
	this->_cgi_manager.setPath(path);
	this->_cgi_flag = 1;
	if (pipe(this->_cgi_fd) < 0) {
		this->_code = 500;
		return (1);
	}
	this->_cgi_manager.setupEnvCgi(this->_request, extension, *this->_server.getLocationByPath(locationpath));
	this->_cgi_manager.executeCgi(this->_code);
	return (0);
}

bool	Response::checkLocation() {
	std::string	locationpath;
	getLocationPath(this->_request.getLocation(), this->_server.getLocation(), locationpath);
	if (locationpath.length() > 0) {
		Location	response_location = *this->_server.getLocationByPath(locationpath);
		if (checkMethod(this->_request.getMethod(), response_location.getMethodsL()))
			return (1);
		if (this->_request.getBody().length() > response_location.getClientBodySizeL()) {
			this->_code = 413;
			return (1);
		}
		if (checkRedirection(response_location))
			return (1);
		if (!response_location.getProxyL().empty())
			replaceProxy(response_location);
		else
			combineRootPath(response_location);
		if (response_location.getPathL().find("cgi-bin") != std::string::npos)
			return (checkCgi(locationpath));
		if (isDir(this->_full_path)) {
			if (this->_full_path[this->_full_path.length() - 1] != '/') {
				this->_location = this->_request.getLocation() + "/";
				this->_code = 301;
				return (1);
			}
			if (!response_location.getIndexL().empty())
				this->_full_path += response_location.getIndexL();
			else
				this->_full_path += this->_server.getIndex();
			if (!realFile(this->_full_path)) {
				if (response_location.getListingL()) {
					this->_full_path.erase(this->_full_path.find_last_of('/') + 1);
					this->_listing = true;
					return (0);
				}
				else {
					this->_code = 403;
					return (1);
				}
			}
			if (isDir(this->_full_path)) {
				if (!response_location.getIndexL().empty())
					this->_location = combinePaths(this->_request.getLocation(), response_location.getIndexL(), "");
				else
					this->_location = combinePaths(this->_request.getLocation(), this->_server.getIndex(), "");
				if (this->_location[this->_location.length() - 1] != '/')
					this->_location.insert(this->_location.end(), '/');
				this->_code = 301;
				return (1);
			}
		}
	}
	else {
		this->_full_path = combinePaths(this->_server.getRoot(), this->_request.getLocation(), "");
		if (isDir(this->_full_path)) {
			if (this->_full_path[this->_full_path.length() - 1] != '/') {
				this->_location = this->_request.getLocation() + "/";
				this->_code = 301;
				return (1);
			}
			this->_full_path += this->_server.getIndex();
			if (!realFile(this->_full_path)) {
				this->_code = 403;
				return (1);
			}
			if (isDir(this->_full_path))
            {
                this->_location = combinePaths(this->_request.getLocation(), this->_server.getIndex(), "");
                if(this->_location[this->_location.length() - 1] != '/')
                    this->_location.insert(this->_location.end(), '/');
				this->_code = 301;
                return (1);
            }
		}
	}
	return (0);
}

bool	Response::buildBody() {
	if (this->_request.getBody().length() > this->_server.getClientBodySize()) {
		this->_code = 413;
		return (1);
	}
	if (checkLocation())
		return (1);
	if (this->_listing == true || this->_code || this->_cgi_flag)
		return (0);
	// std::cout << this->_request.getMethod();
	if (this->_request.getMethod() == "GET") {
		std::ifstream	file(this->_full_path.c_str());
		if (file.fail()) {
			this->_code = 404;
			return (1);
		}
		this->_body = readFile(file);
	}
	else if (this->_request.getMethod() == "POST") {
		// std::cout << "IN POST" << std::endl;
		if (realFile(this->_full_path)) {
			this->_code = 204;
			return (0);
		}
		std::ofstream file(this->_full_path, std::ios::binary);
		if (file.fail()) {
			this->_code = 404;
			return (1);
		}
		if (this->_request.getMultiPart()) {
			std::string body = this->_request.getBody();
			body = handleBoundary(body, this->_request.getBoundary());
			file.write(body.c_str(), body.length());
		}
		else
			file.write(this->_request.getBody().c_str(), this->_request.getBody().length());
	}
	else if (this->_request.getMethod() == "DELETE") {
		if (!realFile(this->_full_path)) {
			this->_code = 404;
			return (1);
		}
		if (remove (this->_full_path.c_str()) != 0) {
			this->_code = 500;
			return (1);
		}
	}
	this->_code = 200;
	return (0);
}

void	Response::buildErrorBody() {
	if (!this->_error_pages.count(this->_code) || this->_error_pages.at(this->_code).empty() || this->_request.getMethod() == "POST" || this->_request.getMethod() == "DELETE")
		this->_body = statusCodes(this->_code);
	else {
		if (this->_code >= 400 && this->_code < 500) {
			this->_location = this->_error_pages.at(this->_code);
			if (this->_location[0] != '/') {
				this->_location.insert(this->_location.begin(), '/');
				this->_code = 302;
			}
		}
		if (!this->_error_pages.count(this->_code) || this->_error_pages.at(this->_code).empty())
			this->_body = statusCodes(this->_code);
		else {
			this->_full_path = this->_server.getRoot() + this->_error_pages.at(this->_code);
			std::ifstream	file(this->_full_path.c_str());
			if (file.fail())
				this->_body = statusCodes(this->_code);
			else
				this->_body = readFile(file);
		}
	}
}

bool	Response::checkErrorCode() {
	if (this->_request.getErrorCode()) {
		this->_code = this->_request.getErrorCode();
		return (1);
	}
	return (0);
}

int	Response::buildDirectoryListing(std::string path, std::string &listingbody) {
	DIR				*dir;
	struct dirent	*item;

	dir = opendir(path.c_str());
	if (dir == NULL)
		return (1);
	listingbody.append("<html>\n<head>\n<title> Index of" + path + "</title>\n</head>\n<body >\n<h1> Index of " + path + "</h1>\n");
	listingbody.append("<table style=\"width:80%; font-size: 20px; border: 2px solid black; border-collapse: collapse\">\n");
    listingbody.append("<th style=\"text-align:left; background-color: lightblue; border: 1px solid black\"> File Name </th>\n");
	listingbody.append("<th style=\"text-align:left; background-color: lightblue; border: 1px solid black\"> File Size </th>\n");
    listingbody.append("<th style=\"text-align:left; background-color: lightblue; border: 1px solid black\"> Last Modification  </th>\n");

    struct stat file_stat;
    std::string file_path;

    while((item = readdir(dir)) != NULL)
    {
        if(strcmp(item->d_name, ".") == 0)
            continue;
        file_path = path + item->d_name;
        stat(file_path.c_str() , &file_stat);
        listingbody.append("<tr>\n<td style = \"border: 1px solid black\">\n<a href=\"");
        listingbody.append(item->d_name);
        if (S_ISDIR(file_stat.st_mode))
            listingbody.append("/");
        listingbody.append("\">");
        listingbody.append(item->d_name);
        if (S_ISDIR(file_stat.st_mode))
            listingbody.append("/");
        listingbody.append("</a>\n</td>\n<td style = \"border: 1px solid black\">\n");
        if (!S_ISDIR(file_stat.st_mode))
            listingbody.append(to_String(file_stat.st_size));
		else
			listingbody.append("Directory");
		listingbody.append("</td>\n<td style = \"border: 1px solid black\">\n");
		listingbody.append(ctime(&file_stat.st_mtime));
        listingbody.append("</td>\n</tr>\n");
    }
    listingbody.append("</table>\n</body>\n</html>\n");
    return (0);
}

void	Response::buildResponse() {
	if (checkErrorCode() || buildBody()) {
		buildErrorBody();
	}
	if (this->_cgi_flag)
		return ;
	if (this->_listing == true) {
		if (buildDirectoryListing(this->_full_path, this->_listingbody)) {
			this->_code = 500;
			buildErrorBody();
			this->defineType();
		}
		else {
			this->_code = 200;
			this->_body.insert(0, this->_listingbody);
			this->_contentType = "Content-Type: text/html; charset=UTF-8\r\n";
		}
	}
	else
		this->defineType();
	this->findLenght();
	this->setDate();
	this->_header.append(this->_conexion);
	this->_status_msg = statusCodes(this->_code);
	buildHeader();
	this->_header.append(this->_conexion);
	this->_header.append(this->_contentType);
	this->_header.append(this->_date);
	this->_header.append(this->_content_lenght);
	this->_header.append("\r\n");
	this->_header.append(this->_body);
}

void	Response::setCgiErrorResponse(int a) {
	this->_header = "";
	this->_code = a;
	this->_body = "";
	buildErrorBody();
	this->findLenght();
	this->setDate();
	this->defineType();
	// this->_contentType = "content-type: text/plain\r\n";
	this->_header.append(this->_conexion);
	this->_status_msg = statusCodes(this->_code);
	buildHeader();
	this->_header.append(this->_conexion);
	this->_header.append(this->_contentType);
	this->_header.append(this->_date);
	this->_header.append(this->_content_lenght);
	this->_header.append("\r\n");
	this->_header.append(this->_body);
}

std::string	Response::getResponse() {
	return(this->_header);
}

void	Response::setServer(Server &server) {
	this->_server = server;
}

Server	Response::getServer() {
	return (this->_server);
}

void	Response::cutResponse(size_t a) {
	this->_header = this->_header.substr(a);
}

void	Response::updateResponse(char *buffer, int a) {
	this->_header.append(buffer, a);
}

void		Response::setCgiFlag(int a) {
	this->_cgi_flag = a;
}

int		Response::getCgiFlag() {
	return (this->_cgi_flag);
}

CgiManager	&Response::getCgiManager() {
	return (this->_cgi_manager);
}

std::string	Response::handleBoundary(std::string content, std::string boundary) {
	std::string translated;
	std::string	hold;
	std::string name;
	bool	bound = false;
	bool	data = false;
	if (content.find("--" + boundary) != std::string::npos && content.find("--" + boundary + "--") != std::string::npos) {
		for (size_t a = 0; a < content.size(); a++) {
			hold.clear();
			while (content[a] != '\n') {
				hold += content[a];
				a++;
			}
			if (!hold.compare(("--" + boundary + "--\r"))) {
				data = true;
				bound = false;
			}
			if (!hold.compare(("--" + boundary + "\r")))
				bound = true;
			if (bound) {
				if (!hold.compare(0, 31, "Content-Disposition: form-data;")) {
					size_t b = hold.find("filename=\"");
					if (b != std::string::npos) {
						size_t c = hold.find("\"", b + 10);
						if (c != std::string::npos)
							name = hold.substr(b + 10, c);
					}
				}
				else if (!hold.compare(0, 1, "\r") && !name.empty()) {
					bound = false;
					data = true;
				}
			}
			else if (data) {
				if (!hold.compare(("--" + boundary + "\r")))
				bound = true;
				else if (!hold.compare(("--" + boundary + "--\r"))) {
					translated.erase(translated.end() - 1);
					break ;
				}
				else
					translated += (hold + "\n");
			}
		}
	}
	content.clear();
	return (translated);
}

void	Response::clearResponse()
{
	this->_code = 0;
	this->_cgi_flag = 0;
	this->_host.clear();
	this->_status_msg.clear();
	this->_header.clear();
	this->_contentType.clear();
	this->_conexion.clear();
	this->_date.clear();
	this->_body.clear();
	this->_content_lenght.clear();
	this->_listing = false;
	this->_location.clear();
	this->_full_path.clear();
	this->_listingbody.clear();
}
