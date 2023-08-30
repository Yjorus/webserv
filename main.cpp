#include "AllHeaders.hpp"
#include "Webserv.hpp"

int	main(int ac, char **av)
{
	if (ac == 1 || ac == 2)
	{
		Webserv	webserv;
		try
		{
			if (ac == 2)
				webserv.config(av[1]);
			else
				webserv.config("..");
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	else
	{
		std::cerr << "Invalid number of arguments!" << std::endl;
		return (1);
	}
	return (0);
}