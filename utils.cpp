#include "AllHeaders.hpp"

unsigned long my_stoul(std::string str)
{
	std::stringstream	ss(str);
	if (str.length() > 10)
		throw std::invalid_argument("argument is not an unsigned long");
	for (size_t a = 0; a < str.size(); a++)
	{
		if (str[a] > 9 || str[a] < 0)
			throw std::invalid_argument("argument is not an unsigned long");
	}
	unsigned long a;
	ss >> a;
	return (a);
}

int checkFile(std::string path)
{
	struct stat	a;

	if (stat(path.c_str(), &a) == 0)
	{
		if (a.st_mode & S_IFREG)
			return (1);
		else if (a.st_mode & s_IFDIR)
			return (2);
		else
			return (3);
	}
	else
		return (-1);
}

int	checkPath(std::string path, int flag)
{
	return (access(path.c_str(), flag));
}