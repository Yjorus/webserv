#ifndef CGIMANAGER_HPP
# define CGIMANAGER_HPP

# include "AllHeaders.hpp"
# include "Request.hpp"
# include "Location.hpp"

class CgiManager
{
		pid_t					_cgi_pid;
		std::string				_body;
		std::map<std::string, std::string>	_cgi_env;
		std::string				_cgi_path;
		int						_exit_code;
		char					**_env;
		char					**_av;
		
	public:

		CgiManager();
		~CgiManager();
		
		void	executeCgi(int &code);
		char	**mapToCStrArray();
		void	setupEnvCgi(Request &request, std::string extension, Location location);
		void	setPath(std::string &path);
		std::string	getPath();
		void	clearCgi();
		pid_t		getPid();

		int						_pipe_cgi_in[2]; // 0 == stdin && 1 == stdout
		int						_pipe_cgi_out[2]; // 0 == stdin && 1 == stdout
};

# endif

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