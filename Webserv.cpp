#include "Webserv.hpp"

Webserv::Webserv()
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

	if (checkFile(conf) == -1)
		throw std::invalid_argument("Not a file or no access");
	content = readContent(conf);
	if (content.empty())
		throw std::invalid_argument("File is empty");
}

int	Webserv::checkFile(std::string path)
{
	struct stat	a;
	if (stat(path.c_str(), &a) == 0 && a.st_mode & S_IFREG)
		return (access(path.c_str(), R_OK));
	else
		return (-1);
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