#include "../inc/Location.hpp"

Location::Location() {
	this->_path = "";
	this->_root = "";
	this->_listing = false;
	this->_index = "";
	this->_redirection = "";
	this->_client_body_size = 0;
	this->_methods.push_back(true);
	this->_methods.push_back(false);
	this->_methods.push_back(false);
}

Location::Location(Location const &copy) {
	if (this != &copy) {
		this->_path = copy._path;
		this->_root = copy._root;
		this->_index = copy._index;
		this->_redirection = copy._redirection;
		this->_listing = copy._listing;
		this->_client_body_size = copy._client_body_size;
		this->_cgi_extensions = copy._cgi_extensions;
		this->_cgi_paths = copy._cgi_paths;
		this->_methods = copy._methods;
		this->_cgimap = copy._cgimap;
	}
	return ;
}

Location::~Location()
{}

Location	&Location::operator=(Location const &assign) {
	if (this != &assign) {
		this->_path = assign._path;
		this->_root = assign._root;
		this->_index = assign._index;
		this->_redirection = assign._redirection;
		this->_listing = assign._listing;
		this->_client_body_size = assign._client_body_size;
		this->_cgi_extensions = assign._cgi_extensions;
		this->_cgi_paths = assign._cgi_paths;
		this->_methods = assign._methods;
		this->_cgimap = assign._cgimap;
	}
	return (*this);
}

void	Location::setPathL(std::string path) {
	this->_path = path;
}

void	Location::setRootL(std::string path) {
	if (checkFile(path) != 2)
		throw std::invalid_argument("root in location is not a directory");
	this->_root = path;
}

void	Location::setIndexL(std::string path) {
	this->_index = path;
}

void	Location::setListingL(std::string option) {
	if (option != "on" && option != "off")
		throw std::invalid_argument("listing must be on or off");
	else
		this->_listing = (option == "on");
}

void	Location::setClientBodySizeL(std::string option) {
	for (size_t a = 0; a < option.size(); a++) {
		if (!std::isdigit(option[a]))
			throw std::invalid_argument("invalid syntax for client body size in location");
	}
	if (!my_stoul(option))
			throw std::invalid_argument("invalid syntax for client body size in location");
	this->_client_body_size = my_stoul(option);
}

void	Location::setClientBodySizeL2(unsigned long option) {
	this->_client_body_size = option;
}

void	Location::setRedirectionL(std::string option) {
	this->_redirection = option;
}

void	Location::setMethodsL(std::vector<std::string> methods) {
	this->_methods[0] = false;
	this->_methods[1] = false;
	this->_methods[2] = false;
	for (size_t a = 0; a < methods.size(); a++) {
		if (methods[a] == "GET" && !this->_methods[0])
			this->_methods[0] = true;
		else if (methods[a] == "POST" && !this->_methods[1])
			this->_methods[1] = true;
		else if (methods[a] == "DELETE" && !this->_methods[2])
			this->_methods[2] = true;
		else
			throw std::invalid_argument("not a valid method or a duplicate method");
	}
}

void	Location::setExtensionsL(std::vector<std::string> extensions) {
	this->_cgi_extensions = extensions;
}

void	Location::setCgiPathL(std::vector<std::string> paths) {
	this->_cgi_paths = paths;
}

void	Location::setCgiMap(std::map<std::string, std::string> map) {
	this->_cgimap = map;
}

std::string					Location::getPathL() const {
	return (this->_path);
}

std::string					Location::getRootL() const {
	return (this->_root);
}

std::string					Location::getIndexL() const {
	return (this->_index);
}

std::string					Location::getRedirectionL() const {
	return (this->_redirection);
}

bool						Location::getListingL() const {
	return (this->_listing);
}

unsigned long				Location::getClientBodySizeL() const {
	return (this->_client_body_size);
}

const std::vector<std::string>	&Location::getCgiExtensionsL() const {
	return (this->_cgi_extensions);
}

const std::vector<std::string>	&Location::getCgiPathsL() const {
	return (this->_cgi_paths);
}

std::vector<bool>			Location::getMethodsL() const {
	return (this->_methods);
}

std::map<std::string, std::string>	Location::getCgiMap() const{
	return (this->_cgimap);
}

std::ostream	&operator<<(std::ostream &o, Location const &location) {
	o << "\npath: " << location.getPathL();
	o << "\nroot: " << location.getRootL();
	o << "\nindex: " << location.getIndexL() << "\n";

	std::map<std::string, std::string>	lmao = location.getCgiMap();
	for (std::map<std::string, std::string>::const_iterator it = lmao.begin(); it != lmao.end(); ++it) {
		o << it->first << " " << it->second << "\n";
	}
	return (o);
}
