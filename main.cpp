/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjorus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 13:30:39 by yjorus            #+#    #+#             */
/*   Updated: 2023/10/24 13:30:40 by yjorus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/AllHeaders.hpp"
#include "inc/Webserv.hpp"
#include "inc/RunServer.hpp"


int	main(int ac, char **av)
{
	if (ac == 1 || ac == 2)
	{
		Webserv	webserv;
		RunServer	run;
		try
		{
			if (ac == 2)
				webserv.config(av[1]);
			else
				webserv.config("example_config");
			run.setupServers(webserv.getServers());
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return (1);
		}
		run.serverLoop();
	}
	else
	{
		std::cerr << "Invalid number of arguments!" << std::endl;
		return (1);
	}
	exit (0);
}
