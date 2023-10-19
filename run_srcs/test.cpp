/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:00:35 by gscarama          #+#    #+#             */
/*   Updated: 2023/10/06 16:08:02 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <fstream>

int main()
{
	std::fstream		file;
	std::string	str;
	std::string	_body;

	file.open("../rootdir/error/404.html");
	while (!file.eof())
	{
		file >> str;
		_body.append(str);
	}
	std::cout << _body;

}