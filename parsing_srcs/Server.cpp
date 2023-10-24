/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjorus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 13:29:48 by yjorus            #+#    #+#             */
/*   Updated: 2023/10/24 13:29:49 by yjorus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server() {
	this->_port = 0;
	this->_host = 0;
	this->_client_body_size = 0;
	this->_index = "";
	this->_server_name = "";
	this->_root = "";
	this->_directory_listing = false;
	this->_fd = 0;
}

Server::Server(Server const &copy) {
	*this = copy;
}

Server::~Server()
{}

Server	&Server::operator=(Server const &assign) {
	if (this != &assign) {
		this->_client_body_size = assign._client_body_size;
		this->_host = assign._host;
		this->_port = assign._port;
		this->_index = assign._index;
		this->_server_name = assign._server_name;
		this->_root = assign._root;
		this->_error_pages = assign._error_pages;
		this->_directory_listing = assign._directory_listing;
		this->_locations = assign._locations;
		this->_fd = assign._fd;
		this->_address = assign._address;
	}
	return (*this);
}

std::vector<std::string>	Server::configSplit(std::string config, std::string separators) {
	std::vector<std::string>	split;
	size_t a = 0,b = 0;

	while (true) {
		b = config.find_first_of(separators, a);
		if (b == std::string::npos)
			break;
		std::string hold = config.substr(a, b - a);
		split.push_back(hold);
		a = config.find_first_not_of(separators, b);
		if (a == std::string::npos)
			break;
	}
	return (split);
}

void	Server::config(std::string config) {
	std::vector<std::string>	split;
	std::vector<std::string>	error_pages;
	split = configSplit(config += ' ', std::string(" \n\t"));
	bool	listing = false;
	bool	locs = false;

	for (size_t a = 0; a < split.size(); a++) {
		if (split[a] == "host" && (a + 1) < split.size() && locs == false) {
			if (this->_host)
				throw std::invalid_argument("Multiple hosts in server block");
			setHost(split[++a]);
		}
		else if (split[a] == "listen" && (a + 1) < split.size() && locs == false) {
			if (this->_port)
				throw std::invalid_argument("Multiple ports in server block");
			setPort(split[++a]);
		}
		else if (split[a] == "root" && (a + 1) < split.size() && locs == false) {
			if (!this->_root.empty())
				throw std::invalid_argument("Multiple roots in server block");
			setRoot(split[++a]);
		}
		else if (split[a] == "server_name" && (a + 1) < split.size() && locs == false) {
			if (!this->_server_name.empty())
				throw std::invalid_argument("Multiple server names in server block");
			setServerName(split[++a]);
		}
		else if (split[a] == "index" && (a + 1) < split.size() && locs == false) {
			if (!this->_index.empty())
				throw std::invalid_argument("Multiple hindexes in server block");
			setIndex(split[++a]);
		}
		else if (split[a] == "client_max_body_size" && (a + 1) < split.size() && locs == false) {
			if (this->_client_body_size)
				throw std::invalid_argument("Multiple client_max_body_size in server block");
			setClientBodySize(split[++a]);
		}
		else if (split[a] == "listing" && (a + 1) < split.size() && locs == false) {
			if (listing == true)
				throw std::invalid_argument("Multiple directory_listings in server block");
			setDirectoryListing(split[++a]);
			listing = true;
		}
		else if (split[a] == "error_pages" && (a + 1) < split.size() && locs == false) {
			while (++a < split.size()) {
				error_pages.push_back(split[a]);
				if (split[a].find(';') != std::string::npos)
					break ;
				else if (a + 1 >= split.size())
					throw std::invalid_argument("Invalid error pages in server block");
			}
		}
		else if (split[a] == "location" && (a + 1) < split.size()) {
			std::string	location_path;

			if (split[++a] == "{" || split[a] == "}")
				throw std::invalid_argument("Invalid location path in server block");
			location_path = split[a];
			if (++a >= split.size() || split[a] != "{")
				throw std::invalid_argument("Invalid location in server block");
			std::vector<std::string>	location_data;
			while (++a < split.size() && split[a] != "}")
				location_data.push_back(split[a]);
			setLocation(location_path, location_data);
			locs = true;
		}
		else if (split[a] != "{" && split[a] != "}")
			throw std::invalid_argument("unsupported argument");
	}
	if (this->_root.empty())
		setRoot("/;");
	if (!this->_host)
		setHost("localhost;");
	if (!this->_port)
		throw std::invalid_argument("No port set");
	if (this->_index.empty())
		setIndex("index.html;");
	if (checkPathAndFile(this->_root, this->_index))
		throw std::invalid_argument("Index not found or readable");
	if (checkDuplicateLocationPaths())
		throw std::invalid_argument("Duplicate location paths");
	setErrorPages(error_pages);
	if (!this->_client_body_size)
		this->_client_body_size = 10000;
}

void	Server::setHost(std::string host) {
	checkSemicolon(host);
	if (host == "localhost")
		host = "127.0.0.1";
	struct in_addr	val;
	if (myInetAton(host.data(), &val))
		this->_host = val.s_addr;
	else
		throw std::invalid_argument("invalid host");
}

void	Server::setPort(std::string port) {
	checkSemicolon(port);
	for (size_t a = 0; a < port.size(); a++) {
		if (!std::isdigit(port[a]))
			throw std::invalid_argument("port syntax wrong");
	}
	int b =  my_stoi(port);
	if (b < 1 || b > 65535)
		throw std::invalid_argument("port syntax wrong");
	this->_port = (uint16_t) b;
}

void	Server::setClientBodySize(std::string size) {
	checkSemicolon(size);
	unsigned long	body = 0;
	for (size_t a = 0; a < size.size(); a++) {
		if (!std::isdigit(size[a]))
			throw std::invalid_argument("body size syntax wrong");
	}
	body = my_stoul(size);
	if (!body)
		throw std::invalid_argument("body size syntax wrong");
	this->_client_body_size = body;
	
}

void	Server::setIndex(std::string index) {
	checkSemicolon(index);
	this->_index = index;
}

void	Server::setServerName(std::string name) {
	checkSemicolon(name);
	this->_server_name = name;
}

void	Server::setRoot(std::string root) {
	checkSemicolon(root);
	if(checkFile(root) == 2)
		this->_root = root;
	else
		throw std::invalid_argument("root argument is not a directory");
}

void	Server::setDirectoryListing(std::string listing) {
	checkSemicolon(listing);
	if (listing != "on" && listing != "off")
		throw std::invalid_argument("invalid directory listing argument");
	else
		this->_directory_listing = (listing == "on");
}

void	Server::setErrorPages(std::vector<std::string> pages) {
	if (pages.empty())
		return ;
	if (pages.size() % 2)
		throw std::invalid_argument("invalid error pages argument");
	for (size_t a = 0; a < pages.size() - 1; a++) {
		for (size_t b = 0; b < pages[a].size(); b++) {
			if (!std::isdigit(pages[a][b]))
				throw std::invalid_argument("error_pages argument is invalid");
		}
		if (pages[a].size() != 3)
			throw std::invalid_argument("error_pages argument is invalid");
		int code = my_stoi(pages[a]);
		if (statusCodes(code) == "WRONG" || code < 400)
			throw std::invalid_argument("error_pages argument is invalid");
		a++;
		if (a == pages.size() - 1)
			checkSemicolon(pages[a]);
		std::string	path = pages[a];
		if (checkFile(this->_root + path) != 1)
			throw std::invalid_argument("error_pages argument is invalid");
		if (checkPath(this->_root + path, 0) == -1 || checkPath(this->_root + path, 4) == -1)
			throw std::invalid_argument("error_pages argument is invalid");
		std::map<int, std::string>::iterator it = this->_error_pages.find(code);
		if (it != this->_error_pages.end())
			throw std::invalid_argument("duplicate error page");
		this->_error_pages.insert(std::make_pair(code, path));
	}
}

void	Server::setLocation(std::string path, std::vector<std::string> data) {
	Location	location;
	bool	listing  = false;
	bool	method = false;
	location.setPathL(path);
	for (size_t a = 0; a < data.size(); a++) {
		if (data[a] == "listing" && a + 1 < data.size()) {
			if (path == "/cgi-bin/")
				throw std::invalid_argument("listing not allowed in cgi-bin");
			if (listing == true)
				throw std::invalid_argument("duplicate listing argument in location");
			checkSemicolon(data[++a]);
			location.setListingL(data[a]);
			listing = true;
		}
		else if (data[a] == "root" && a + 1 < data.size()) {
			if (!location.getRootL().empty())
				throw std::invalid_argument("duplicate root argument in location");
			checkSemicolon(data[++a]);
			if (checkFile(data[a]) == 2)
				location.setRootL(data[a]);
			else if (checkFile(this->_root + data[a]) == 2)
				location.setRootL(this->_root + data[a]);
			else
				throw std::invalid_argument("root argument in location is not a directory");
		}
		else if (data[a] == "index" && a + 1 < data.size()) {
			if (!location.getIndexL().empty())
				throw std::invalid_argument("duplicate index argument in location");
			checkSemicolon(data[++a]);
			location.setIndexL(data[a]);
		}
		else if (data[a] == "redirection" && a + 1 < data.size()) {
			if (path == "/cgi-bin/")
				throw std::invalid_argument("redirection not allowed in cgi-bin");
			if (!location.getRedirectionL().empty())
				throw std::invalid_argument("duplicate redirection argument in location");
			checkSemicolon(data[++a]);
			location.setRedirectionL(data[a]);
		}
		else if (data[a] == "proxy" && (a + 1) < data.size())
		{
			if (path == "/cgi-bin/")
				throw std::invalid_argument("proxy not allowed in cgi-bin");
			if (!location.getProxyL().empty())
				throw std::invalid_argument("duplicate proxy argument in location");
			checkSemicolon(data[++a]);
			location.setProxyL(data[a]);
		}
		else if (data[a] == "methods" && a + 1 < data.size()) {
			if (method == true)
				throw std::invalid_argument("duplicate methods argument in location");
			std::vector<std::string>	methods;
			while (++a < data.size()) {
				if (data[a].find(";") != std::string::npos) {
					checkSemicolon(data[a]);
					methods.push_back(data[a]);
					break;
				}
				methods.push_back(data[a]);
				if (a + 1 >= data.size())
					throw std::invalid_argument("no semicolon in methods argument in location");
			}
			location.setMethodsL(methods);
			method = true;
		}
		else if (data[a] == "client_max_body_size" && a + 1 < data.size()) {
			if (location.getClientBodySizeL())
				throw std::invalid_argument("duplicate body_size argument in location");
			checkSemicolon(data[++a]);
			location.setClientBodySizeL(data[a]);
		}
		else if (data[a] == "cgi_extensions" && a + 1 < data.size()) {
			std::vector<std::string>	extensions;
			while (++a < data.size()) {
				if (data[a].find(";") != std::string::npos) {
					checkSemicolon(data[a]);
					extensions.push_back(data[a]);
					break;
				}
				extensions.push_back(data[a]);
				if (a + 1 >= data.size())
					throw std::invalid_argument("no semicolon in extensions argument in location");
			}
			location.setExtensionsL(extensions);
		}
		else if (data[a] == "cgi_path" && a + 1 < data.size()) {
			std::vector<std::string>	cgi_paths;
			while (++a < data.size()) {
				if (data[a].find(";") != std::string::npos) {
					checkSemicolon(data[a]);
					cgi_paths.push_back(data[a]);
					break;
				}
				cgi_paths.push_back(data[a]);
				if (a + 1 >= data.size())
					throw std::invalid_argument("no semicolon in cgi_paths argument in location");
			}
			location.setCgiPathL(cgi_paths);
		}
	}
	if (!location.getClientBodySizeL()) {
		if (this->_client_body_size)
			location.setClientBodySizeL2(this->_client_body_size);
		else
			location.setClientBodySizeL2(1000);
	}
	if (path != "/cgi-bin/" && location.getIndexL().empty()) {
		location.setIndexL(this->_index);
	}
	int check = locationCheck(location);
	if (check == 1)
		throw std::invalid_argument("invalid path for location");
	if (check == 2)
		throw std::invalid_argument("index of location not readable");
	if (check == 3)
		throw std::invalid_argument("invalid redirection in location");
	if (check == 4)
		throw std::invalid_argument("invalid cgi parameters");
	this->_locations.push_back(location);
}

void						Server::setFd(int value) {
	this->_fd = value;
}

int		Server::locationCheck(Location &location)const {
	if (location.getPathL() == "/cgi-bin/") {
		if (location.getCgiPathsL().empty() || location.getCgiExtensionsL().empty() || location.getIndexL().empty())
			return (4);
		if (location.getRootL().empty()) {
			if (!this->_root.empty())
				location.setRootL(this->_root);
			else
				location.setRootL("/");
		}
		if (location.getCgiPathsL().size() != location.getCgiExtensionsL().size() || location.getCgiPathsL().size() > 2)
			return (4);
		std::vector<std::string>::const_iterator it;
		for (it = location.getCgiPathsL().begin(); it != location.getCgiPathsL().end(); ++it) {
			if (checkFile(*it) < 0)
				return (4);
		}
		std::vector<std::string>::const_iterator it2;
		std::map<std::string, std::string> temp_map;
		for (it = location.getCgiExtensionsL().begin(); it != location.getCgiExtensionsL().end(); ++it) {
			std::string	hold = *it;
			if ( hold != ".pl" && hold != ".py") {
				return (4);
			}
			bool	lmao = false;
			for (it2 = location.getCgiPathsL().begin(); it2 != location.getCgiPathsL().end(); it2++) {
				std::string hold2 = *it2;
				if (hold == ".pl" && hold2 != "/usr/bin/perl") {
					if (lmao)
						return (4);
					lmao = true;
				}
				else if (hold == ".pl") {
					if (temp_map.count(".pl"))
						return (4);
					temp_map.insert(std::make_pair(".pl", hold2));
					}
				if (hold == ".py" && hold2 != "/usr/bin/python3") {
					if (lmao)
						return (4);
					lmao = true;
				}
				else if (hold == ".py") {
					if (temp_map.count(".py"))
						return (4);
					temp_map.insert(std::make_pair(".py", hold2));
				}
			}
		}
		location.setCgiMap(temp_map);
	}
	else {
		if (location.getPathL()[0] != '/' || location.getPathL()[location.getPathL().size() - 1] != '/')
			return (1);
		if (location.getRootL().empty()) {
			if (!this->_root.empty())
				location.setRootL(this->_root);
			else
				location.setRootL("/");
		}
		if (checkPathAndFile(location.getRootL() + location.getPathL() + "/", location.getIndexL()))
			return (5);
		if (!location.getRedirectionL().empty() && checkPathAndFile(location.getRootL(), location.getRedirectionL()))
			return (3);
		if (!location.getProxyL().empty() && checkPathAndFile(location.getRootL(), location.getProxyL()))
			return (3);
	}
	return (0);
}

int		Server::checkDuplicateLocationPaths(void)const {
	if (this->_locations.size() < 2)
		return (0);
	std::vector<Location>::const_iterator	it;
	std::vector<Location>::const_iterator	it2;
	for (it = this->_locations.begin(); it != this->_locations.end(); it++) {
		for (it2 = it + 1; it2 != this->_locations.end(); it2++) {
			if (it->getPathL() == it2->getPathL())
				return (1);
		}
	}
	return (0);
}

void	Server::checkSemicolon(std::string &str) {
	size_t a = str.rfind(';');
	if (a != str.size() - 1)
		throw std::invalid_argument("Semicolon missing or in incorrect location");
	str.erase(a);
}

uint16_t	Server::getPort()const {
	return(this->_port);
}

uint32_t	Server::getHost()const {
	return (this->_host);
}

unsigned long	Server::getClientBodySize()const {
	return (this->_client_body_size);
}

std::string	Server::getIndex()const {
	return (this->_index);
}

std::string	Server::getServerName()const {
	return (this->_server_name);
}

std::string	Server::getRoot()const {
	return (this->_root);
}

std::map<int, std::string>	Server::getErrorPages()const {
	return (this->_error_pages);
}

bool	Server::getDirectoryListing()const {
	return (this->_directory_listing);
}

int	Server::getFd()const {
	return (this->_fd);
}

std::vector<Location>	Server::getLocation()const {
	return (this->_locations);
}

void	Server::prepareServer() {
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
		throw std::invalid_argument("Failed to init socket");
	int	option = 1;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
	memset(&_address, 0, sizeof(_address));
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr = _host;
	if (bind(_fd, (struct sockaddr *) &_address, sizeof(_address)) == -1) {
		std::string error = "failed to bind socket: ";
		throw std::invalid_argument(error + strerror(errno));
	}
}

std::vector<Location>::iterator	Server::getLocationByPath(std::string path) {
	std::vector<Location>::iterator it;
	for (it = this->_locations.begin(); it != this->_locations.end(); it++) {
		if (it->getPathL() == path)
			return (it);
	}
	throw std::invalid_argument(strerror(errno));
}

struct sockaddr_in	Server::getAddress() const
{
	return(this->_address);
}

std::ostream	&operator<<(std::ostream &o, Server const &server) {
	o << "\nport: " << server.getPort();
	o << "\nhost: " << server.getHost();
	o << "\nbody_size: " << server.getClientBodySize();
	o << "\nindex: " << server.getIndex();
	o << "\nserverName: " << server.getServerName();
	o << "\nroot: " << server.getRoot();
	o << "\nerror_pages: ";
	std::map<int, std::string>	lmao = server.getErrorPages();
	for (std::map<int, std::string>::const_iterator it = lmao.begin(); it != lmao.end(); ++it) {
		o << it->first << " " << it->second << "\n";
	}
	std::vector<Location> lol = server.getLocation();
	for (std::vector<Location>:: const_iterator it2 = lol.begin(); it2 != lol.end(); ++it2) {
		o << *it2 << "\n";
	}
	o << "Listing: " << server.getDirectoryListing() << std::endl;
	return (o);
}
