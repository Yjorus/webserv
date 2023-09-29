#include "../inc/Webserv.hpp"

Webserv::Webserv(): _server_amount(0), _epollfd(0), _fdamount(0)
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
		std::cout << server;
	}
	if (_server_amount > 1)
		checkDuplicateServers();
}

void	Webserv::checkDuplicateServers()
{
	std::vector<Server>::iterator it;
	std::vector<Server>::iterator it2;
	for (it = this->_servers.begin(); it != this->_servers.end() - 1; it++)
	{
		for (it2 = it + 1; it2 != this->_servers.end(); it2++)
		{
			if (it->getPort() == it2->getPort() && it->getHost() == it2->getHost() && it->getServerName() == it2->getServerName())
				throw std::invalid_argument("Duplicate server found");
		}
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

void	Webserv::initEvents(struct epoll_event *ev, uint32_t flag, int fd)
{
	memset(ev, 0, sizeof(ev));
	ev->events = flag;
	ev->data.fd = fd;
}

void	Webserv::makeServerMap()
{
	for (std::vector<Server>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
	{
		if (listen(it->getFd(), 512) < 0)
			exit(1);
		if (fcntl(it->getFd(), F_SETFL, O_NONBLOCK) < 0)
			exit(1);
		_servermap.insert(std::make_pair(it->getFd(), *it));
	}
}

void	Webserv::setupServers()
{
	bool	a = false;
	for (std::vector<Server>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
	{
		a = false;
		for (std::vector<Server>::iterator it2 = this->_servers.begin(); it2 != it; it2++)
		{
			if (it->getHost() == it2->getHost() && it->getPort() == it2->getPort())
			{
				a = true;
				it->setFd(it2->getFd());
			}
		}
		if (a == false)
			it->prepareServer();
	}
}

int	Webserv::sameFd(int a)
{
	for (std::map<int, Server>::iterator it = this->_servermap.begin(); it != this->_servermap.end(); it++)
	{
		if (it->getFd() == a)
			return (a);
	}
	return (-1);
}

void	Webserv::connectClient(int a)
{
	struct epoll_event	ev;
	struct sockaddr		client_addr;
	socklen_t			client_len = sizeof(client_addr);
	int					client_fd;
	Client				new_client;

	if ((client_fd  = accept(a, (struct sockaddr_in *)&client_addr, &client_len)) < 0)
		return ;
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		close(client_fd);
		return ;
	}
	initEvents(&ev, EPOLLIN, client_fd);
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
	{
		close(client_fd);
		return ;
	}
	_connections.insert(make_pair(client_fd, a));
	return ;
}

void	Webserv::initEpoll()
{
	struct epoll_event ev;
	this->_epollfd = epoll_create(10);
	if (this->_epollfd < 0)
		exit (1);
	for (std::map<int, Server>::iterator it = this->_servermap.begin(); it != this->_servermap.end(); it++)
	{
		initEvents(&ev, EPOLLIN, it->getFd());
		if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, it->getFd(), &ev) == -1)
			exit(1);
	}
	initEvents(&ev, EPOLLIN, 0);
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, 0, &ev) == -1)
		exit(1);

}

void	Webserv::runWebserv()
{
	makeServerMap();
	initEpoll();
	while (1)
	{
		struct	epoll_event	events[10];
		int		fds = 0;
		int		a = 0;

		if ((fds = epoll_wait(_epollfd, events, 10, -1)) == -1)
			break ;
		for (int b = 0; b < fds; b++)
		{
			if ((events[b].events & EPOLLHUP) || (events[b].events & EPOLLERR) || !(events[b].events & EPOLLIN))
			{
				close(events[b].data.fd);
				continue;
			}
			else if ((a = sameFd(events[b].data.fd)) >= 0)
				connectClient(a);
			else if (events[b].data.fd == 0)
			{
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else
			{
				handleRequest(events[b].data.fd)
			}
		}
	}
}

std::vector<Server>	Webserv::getServers()
{
	return (this->_servers);
}
