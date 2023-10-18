/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiManager.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 14:19:29 by gscarama          #+#    #+#             */
/*   Updated: 2023/10/17 14:27:05 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AllHeaders.hpp"
#include "Request.hpp"
#include "Location.hpp"

class CgiManager
{
	private:
		pid_t								_cgi_pid;
		std::string							_body;
		std::map<std::string, std::string>	_cgi_env;
		char**								_cp_env;
		int									_execve_return_val;
		std::string							_loc_cgi;
		char**								_argv;
	public:
		void	initCGI(Request &request, const std::vector<Location>::iterator location);
		char	**mapToCStrArray();
		void	setupEnvCgi(Request &request);
};

//needed for cgi : av[0] == path av[1]

//    "COMSPEC", "DOCUMENT_ROOT", "GATEWAY_INTERFACE",   
//    "HTTP_ACCEPT", "HTTP_ACCEPT_ENCODING",             
//    "HTTP_ACCEPT_LANGUAGE", "HTTP_CONNECTION",         
//    "HTTP_HOST", "HTTP_USER_AGENT", "PATH",            
//    "QUERY_STRING", "REMOTE_ADDR", "REMOTE_PORT",      
//    "REQUEST_METHOD", "REQUEST_URI", "SCRIPT_FILENAME",
//    "SCRIPT_NAME", "SERVER_ADDR", "SERVER_ADMIN",      
//    "SERVER_NAME","SERVER_PORT","SERVER_PROTOCOL",     
//    "SERVER_SIGNATURE","SERVER_SOFTWARE"  
// source : https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm