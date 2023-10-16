#include "../inc/AllHeaders.hpp"

unsigned long my_stoul(std::string str) {
	std::stringstream	ss(str);
	if (str.length() > 10)
		throw std::invalid_argument("argument is not an unsigned long");
	for (size_t a = 0; a < str.size(); a++) {
		if (!std::isdigit(str[a]))
			throw std::invalid_argument("argument is not an unsigned long");
	}
	unsigned long a;
	ss >> a;
	return (a);
}

int my_stoi(std::string str) {
	std::stringstream	ss(str);
	if (str.length() > 10)
		throw std::invalid_argument("1argument is not an int");
	for (size_t a = 0; a < str.size(); a++) {
		if (!std::isdigit(str[a]))
			throw std::invalid_argument("2argument is not an int");
	}
	int a;
	ss >> a;
	return (a);
}

std::string	readFile(std::ifstream &file)
{
	std::ostringstream	str;

	str << file.rdbuf();
	file.close();
	return (str.str());
}

std::string	to_String(int nbr)
{
	std::string			str;
	std::stringstream	ss;

	ss << nbr;
	ss >> str;
	return (str);
}

int checkFile(std::string path) {
	struct stat	a;

	if (stat(path.c_str(), &a) == 0) {
		if (a.st_mode & S_IFREG)
			return (1);
		else if (a.st_mode & S_IFDIR)
			return (2);
		else
			return (3);
	}
	else
		return (-1);
}

int	checkPath(std::string path, int flag) {
	return (access(path.c_str(), flag));
}

int	checkPathAndFile(std::string path, std::string file) {
	if (checkFile(file) == 1 && checkPath(file, 4) == 0)
		return (0);
	if (checkFile(path + "/" + file) == 1 && checkPath(path + "/" + file, 4) == 0)
		return (0);
	return (1);
}

std::string	statusCodes(int code) {
	switch (code) {
		case 100:
			return ("Continue");
		case 101:
			return ("Switching Protocols");
		case 200:
			return ("OK");
		case 201:
			return ("Created");
		case 202:
			return ("Accepted");
		case 203:
			return ("Non-Authoritative Information");
		case 204:
			return ("No Content");
		case 205:
			return ("Reset Content");
		case 206:
			return ("Partial Content");
		case 300:
			return ("Multiple Choices");
		case 301:
			return ("Moved Permanently");
		case 302:
			return ("Found");
		case 303:
			return ("See Other");
		case 304:
			return ("Not Modified");
		case 305:
			return ("Use Proxy");
		case 306:
			return ("(Unused)");
		case 307:
			return ("Temporary Redirect");
		case 308:
			return ("Permanent Redirect");
		case 400:
			return ("Bad Request");
		case 401:
			return ("Unauthorized");
		case 402:
			return ("Payment Required");
		case 403:
			return ("Forbidden");
		case 404:
			return ("Not Found");
		case 405:
			return ("Method Not Allowed");
		case 406:
			return ("Not Acceptable");
		case 407:
			return ("Proxy Authentication Required");
		case 408:
			return ("Request Timeout");
		case 409:
			return ("Conflict");
		case 410:
			return ("Gone");
		case 411:
			return ("Length Required");
		case 412:
			return ("Precondition Failed");
		case 413:
			return ("Content Too Large");
		case 414:
			return ("URI Too Long");
		case 415:
			return ("Unsupported Media Type");
		case 416:
			return ("Range Not Satisfiable");
		case 417:
			return ("Expectation Failed");
		case 418:
			return ("(Unused)");
		case 421:
			return ("Misdirected Request");
		case 422:
			return ("Unprocessable Content");
		case 426:
			return ("Upgrade Required");
		case 500:
			return ("Internal Server Error");
		case 501:
			return ("Not Implemented");
		case 502:
			return ("Bad Gateway");
		case 503:
			return ("Service Unavailable");
		case 504:
			return ("Gateway Timeout");
		case 505:
			return ("HTTP Version Not Supported");
		default:
			return ("WRONG");
	}
}

int myInetAton(const char *cp, struct in_addr *addr) {
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
			}
			else
				return (0);
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
			if (val > 0xff || parts[0] > 0xff || parts[1] > 0xff || parts[2] > 0xff)
				return (0);
			val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
			break;
	}
	if (addr != NULL)
		addr->s_addr = htonl(val);
	return (1);
}
