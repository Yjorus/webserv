#include "Server.hpp"

Server::Server()
{
	this->_port = "";
	this->_host = "";
	this->_client_body_size = 0;
	this->_index = "";
	this->_server_name = "";
	this->_root = "";
	this->_directory_listing = false;
}

Server::Server(Server const &copy)
{}

Server::~Server()
{}

Server	&Server::operator=(Server const &assign)
{}

std::vector<std::string>	Server::configSplit(std::string config, std::string separators)
{
	std::vector<std::string>	split;
	size_t a,b = 0;

	while (true)
	{
		b = config.find_first_of(seperators, a);
		if (b == std::string::npos)
			break;
		std::string hold = config.substr(a, b - a);
		split.push_back(hold);
		a = config.find_first_not_of(seperators, b);
		if (a == std::string::npos)
			break;
	}
	return (split);
}

void	Server::config(std::string config)
{
	std::vector<std::string>	split;
	std::vector<std::string>	error_pages;
	split = configSplit(config += ' ', std::string(" \n\t"));
	bool	listing = false;

	for (size_t a = 0; a < split.size(); a++)
	{
		if (split[a] == "host" && (a + 1) < split.size())
		{
			if (!this->_host.empty())
				throw std::invalid_argument("Multiple hosts in server block");
			setHost(split[++a]);
		}
		else if (split[a] == "listen" && (a + 1) < split.size())
		{
			if (!this->_port.empty())
				throw std::invalid_argument("Multiple ports in server block");
			setPort(split[++a]);
		}
		else if (split[a] == "root" && (a + 1) < split.size())
		{
			if (!this->_root.empty())
				throw std::invalid_argument("Multiple roots in server block");
			setRoot(split[++a]);
		}
		else if (split[a] == "server_name" && (a + 1) < split.size())
		{
			if (!this->_server_name.empty())
				throw std::invalid_argument("Multiple server names in server block");
			setServerName(split[++a]);
		}
		else if (split[a] == "index" && (a + 1) < split.size())
		{
			if (!this->_index.empty())
				throw std::invalid_argument("Multiple hindexes in server block");
			setIndex(split[++a]);
		}
		else if (split[a] == "client_max_body_size" && (a + 1) < split.size())
		{
			if (this->_client_body_size)
				throw std::invalid_argument("Multiple client_max_body_size in server block");
			setClientBodySize(split[++a]);
		}
		else if (split[a] == "listing" && (a + 1) < split.size())
		{
			if (listing == true)
				throw std::invalid_argument("Multiple directory_listings in server block");
			setDirectoryListing(split[++a]);
			listing = true;
		}
		else if (split[a] == "error_pages" && (a + 1) < split.size())
		{
			while (++a < split.size())
			{
				error_pages.push_back(split[a]);
				if (split[a].find(';') != std::string::npos)
					break ;
				else if (a + 1 >= split.size())
					throw std::invalid_argument("Invalid error pages in server block");
			}
		}
		else if (split[a] == "location" && (a + 1) < split.size())
		{
			std::string	location_path;

			if (split[++a] == "{" || split[a] == "}")
				throw std::invalid_argument("Invalid location path in server block");
			location_path = split[a];
			if (++a >= split.size() || split[a] != "{")
				throw std::invalid_argument("Invalid location in server block");
			std::vector<std::string>	location_data;
			while (++a < split.size() && split[a] != "}")
				location.data.push_back(split[a]);
			setLocation(location_path, location_data);
		}
		else if (split[a] != "{" && split[a] != "}")
			throw std::invalid_argument("unsupported argument");
	}
	if (this->_root.empty())
		setRoot("/;");
	if (this->_host.empty())
		setHost("localhost;");
	if (this->_port.empty())
		throw std::invalid_argument("No port set");
	if (this->_index.empty())
		setIndex("index.html;");
	if (checkRootAndIndex())
		throw std::invalid_argument("Index not found or readable");
	setErrorPages(error_pages);

}

void	Server::setHost(std::string host)
{
	checkSemicolon(index);
	if (host == "localhost")
		this->_host = "127.0.0.1";
	else
		this->_host = host;
}	

// void	Server::setHost(std::string ip)
// {
// 	checkSemicolon(ip);
// 	if (ip == "localhost")
// 		ip = "127.0.0.1";
// 	if (checkHost(ip += '.'))
// 		throw std::invalid_argument("invalid host");
// 	std::istringstream	ss(ip);
// 	int 				a,b,c,d;
// 	char				e;
// 	ss >> a >> e >> b >> e >> c >> e >> d;
// 	if (!(a < 256 && b < 256 && c < 256 && d < 256))
// 		throw std::invalid_argument("invalid host");
// 	this->_host = (a << 24) + (b << 16) + (c << 8) + d;
// 	return ; 
// }

// bool	Server::checkHost(std::string ip)
// {
// 	size_t	a,b = 0;
// 	int d;
// 	char e;
// 	std::vector<std::string>	split;
// 	while (true)
// 	{
// 		b = ip.find(".", a);
// 		if (b == std::string::npos)
// 			break;
// 		std::string hold = ip.substr(a, b - a);
// 		split.push_back(hold);
// 		if (b + 1 >= ip.size())
// 			break;
// 		a = b + 1;
// 	}
// 	if (split.size() != 4)
// 		return (true);
// 	for (a = 0; a < 4; a++)
// 	{
// 		d = -1;
// 		std::stringstream ss(split[a]);
// 		ss >> d >> e;
// 		if (d < 0 || d > 255 || e)
// 			return (true);
// 	}
// 	return (false);
// }

void	Server::setPort(std::string port)
{
	int  a = 0;

	checkSemicolon(port);
	this->_port = port;
}

void	Server::setClientBodySize(std::string size)
{
	checkSemicolon(size);
	unsigned long	body = 0;
	for (size_t a = 0; a < size.size(); a++)
	{
		if (size[a] > 9 || size[a] < 0)
			throw std::invalid_argument("body size syntax wrong");
	}
	body = my_stoul(size);
	if (!body)
		throw std::invalid_argument("body size syntax wrong");
	this->_client_body_size = body;
	
}

void	Server::setIndex(std::string index)
{
	checkSemicolon(index);
	this->_index = index;
}

void	Server::setServerName(std::string name)
{
	checkSemicolon(name);
	this->_server_name = name;
}

void	Server::setRoot(std::string root)
{
	checkSemicolon(root);
	if(checkFile(root) == 2)
		this->_root = root;
	else
		throw std::invalid_argument("root argument is not a directory");
}

void	Server::setDirectoryListing(std::string listing)
{
	checkSemicolon(listing);
	if (listing != "on" && listing != "off")
		throw std::invalid_argument("invalid directory listing argument");
	if (listing == "on")
		this->_directory_listing = true;
}

void	Server::setErrorPages(std::vector<std::string> pages)
{
	if (pages.empty())
		return ;
	if (pages.size() % 2)
		throw std::invalid_argument("invalid error pages argument");
	for (size_t a = 0; a < pages.size() - 1; a++)
	{
		for (size_t b = 0; b < pages[a].size; b++)
		{
			if (!std::isdigit(pages[a][b]))
				throw std::invalid_argument("error_pages argument is invalid");
		}
		if (pages[a].size != 3)
			throw std::invalid_argument("error_pages argument is invalid");
		int code = my_stoi(pages[a]);
		if (statusCodes(code) == "WRONG" || codes < 400)
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
			this_error_pages[code] = path;
		else
			this->_error_pages.insert(std::make_pair(code, path));
	}
}

void	Server::setLocation(std::string path, std::vector<std::string> data)
{}

void	Server::checkSemicolon(std::string &str)
{
	size_t a = str.rfind(';');
	if (a != str.size() - 1)
		throw std::invalid_argument("Semicolon missing or in incorrect location");
	str.erase(a);
}

std::string	Server::getPort()const
{
	return(this->_port);
}

ustd::string	Server::getHost()const
{
	return (this->_host);
}

unsigned long	Server::getClientBodySize()const
{
	return (this->_client_body_size);
}

std::string	Server::getIndex()const
{
	return (this->_index);
}

std::string	Server::getServerName()const
{
	return (this->_server_name);
}

std::string	Server::getRoot()const
{
	return (this->_root);
}

std::map<int, std::string>	Server::getErrorPages()const
{
	return (this->_error_pages);
}

bool	Server::getDirectoryListing()const
{
	return (this->_directory_listing);
}
