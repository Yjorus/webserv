#include "Webserv.hpp"

Webserv::Webserv(): _server_amount(0)
{}

// Webserv::Webserv(Webserv const &copy)
// {}

Webserv::~Webserv()
{}

// Webserv	&Webserv::operator=(Webserv const &assign)
// {}

void	Webserv::config(std::string conf)
{
	std::string	content;

	if (checkFile(conf) != 1)
		throw std::invalid_argument("Not a regular file");
	if (checkPath(conf, R_OK))
		throw std::invalid_argument("No access");
	content = readContent(conf);
	if (content.empty())
		throw std::invalid_argument("File is empty");
	storeServerBlocks(content);
	for (int b = 0; b < _server_amount; b++)
	{
		Server	server;
		server.config(_server_blocks[b]);
		_servers.push_back(server);
		std::cout << _server_blocks[b] << std::endl;
	}
}

std::string	Webserv::readContent(std::string path)
{
	if (path.empty() || path.length() == 0)
		return (NULL);
	std::ifstream	conf(path.c_str());
	if (!conf | !conf.is_open())
		return (NULL);
	std::stringstream	content;
	content << conf.rdbuf();
	return (content.str());
}

void	Webserv::storeServerBlocks(std::string &content)
{
	size_t p;
	size_t p2;

	p = content.find('#');
	while (p != std::string::npos)
	{
		p2 = content.find('\n', p);
		content.erase(p, p2 - p);
		p = content.find('#');
	}
	p = 0;
	while (content[p] && isspace(content[p]))
		p++;
	content = content.substr(p);
	p = content.length() - 1;
	while (content[p] && isspace(content[p]))
		p--;
	content = content.substr(0, p + 1);
	p = 0;
	p2 = 1;
	if (content.find("server", 0) == std::string::npos)
		throw std::invalid_argument("File has no server block");
	while (p < content.length())
	{
		p = findBlockStart(p, content);
		p2 = findBlockEnd(p, content);
		if (p == p2)
			throw std::invalid_argument("Server block doesn't close");
		this->_server_blocks.push_back(content.substr(p, p2 - p + 1));
		this->_server_amount++;
		p = p2 + 1;
	}
}

size_t	Webserv::findBlockStart(size_t a, std::string &content)
{
	size_t	p;

	for (p = a; content[p]; p++)
	{
		if (content[p] == 's')
			break;
		if (!isspace(content[p]))
			throw std::invalid_argument("Invalid characters outside of server block scope");
	}
	if (!content[p])
		return (a);
	if (content.compare(p, 6, "server") != 0)
		throw std::invalid_argument("Invalid characters outside of server block scope");
	p += 6;
	while (content[p] && isspace(content[p]))
		p++;
	if (content[p] == '{')
		return (p);
	else
		throw std::invalid_argument("Invalid characters outside of server block scope");
}

size_t	Webserv::findBlockEnd(size_t a, std::string &content)
{
	size_t	p;
	size_t	brackets = 0;

	for (p = a + 1; content[p]; p++)
	{
		if (content[p] == '{')
			brackets++;
		if (content[p] == '}')
		{
			if (brackets == 0)
				return (p);
			brackets--;
		}
	}
	return (a);
}