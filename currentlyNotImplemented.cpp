bool	Server::checkHost(std::string ip)
{
	size_t	a,b = 0;
	int d;
	char e;
	std::vector<std::string>	split;
	while (true)
	{
		b = ip.find(".", a);
		if (b == std::string::npos)
			break;
		std::string hold = ip.substr(a, b - a);
		split.push_back(hold);
		if (b + 1 >= ip.size())
			break;
		a = b + 1;
	}
	if (split.size() != 4)
		return (true);
	for (a = 0; a < 4; a++)
	{
		d = -1;
		std::stringstream ss(split[a]);
		ss >> d >> e;
		if (d < 0 || d > 255 || e)
			return (true);
	}
	return (false);
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

	// struct	addrinfo hints;
	// memset(&hints, 0 , sizeof hints);
	// hints.ai_family = AF_INET;
	// hints.ai_socktype = SOCK_STREAM;
	// struct addrinfo *servinfo;
	// int status = getaddrinfo(this->getHost().c_str(), this->getPort().c_str(), &hints, &servinfo);
	// if (status)
	// 	throw std::invalid_argument(gai_strerror(status));
	// this->_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	// if (this->_fd == -1)
	// {
	// 	freeaddrinfo(servinfo);
	// 	throw std::invalid_argument("Failed to init socket");
	// }
	// if( bind(this->_fd, servinfo->ai_addr, servinfo->ai_addrlen) < 0 )
	// {
    // 	close(this->_fd);
    // 	freeaddrinfo(servinfo);
	// 	throw std::invalid_argument("Failed to bind socket");
	// }
	// std::cout << "correct" << std::endl;
	// freeaddrinfo(servinfo);

int myInetAton(const char *cp, struct in_addr *addr)
{
	u_long parts[4];
	in_addr_t val = 0;
	const char *c;
	char *endptr;
	int gotend, n;

	c = (const char *)cp;
	n = 0;

	gotend = 0;
	while (!gotend) {
		unsigned long l;
		l = strtoul(c, &endptr, 0);

		if (l == ULONG_MAX || (l == 0 && endptr == c))
			return (0);

		val = (in_addr_t)l;

		if (endptr == c)
			return (0);
		parts[n] = val;
		c = endptr;

		switch (*c) {
		case '.' :
			if (n == 3)
				return (0);
			n++;
			c++;
			break;

		case '\0':
			gotend = 1;
			break;

		default:
			if (isspace((unsigned char)*c)) {
				gotend = 1;
				break;
			} else {

				/* Invalid character, then fail. */
				return (0);
			}
		}

	}

	switch (n) {
	case 0:	
		break;
	case 1:	
		if (val > 0xffffff || parts[0] > 0xff)
			return (0);
		val |= parts[0] << 24;
		break;

	case 2:	
		if (val > 0xffff || parts[0] > 0xff || parts[1] > 0xff)
			return (0);
		val |= (parts[0] << 24) | (parts[1] << 16);
		break;

	case 3:	
		if (val > 0xff || parts[0] > 0xff || parts[1] > 0xff ||
		    parts[2] > 0xff)
			return (0);
		val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
		break;
	}
	if (addr != NULL)
		addr->s_addr = htonl(val);
	return (1);
}

void	Webserv::setupServers()
{
	bool	a = false;
	_epollfd = epoll_create(10);
	if (_epollfd == -1)
		throw std::invalid_argument("Failed to create epoll fd");
	struct  epoll_event	ev;
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
		{
			it->prepareServer();
			initEvents(&ev, EPOLLIN, it->getFd());
			if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, it->getFd(), &ev) == -1)
				throw std::invalid_argument("Failed to add entry to interest list of epoll");
		}
	}
	initEvents(&ev, EPOLLIN, 0);
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, 0, &ev) == -1)
				throw std::invalid_argument("Failed to add entry to interest list of epoll");
}