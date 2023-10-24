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
		time_t					_timeout;
		
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
		int		findstart(std::string path, std::string key);
		void	checkTimeout();

		int						_pipe_cgi_in[2];
		int						_pipe_cgi_out[2];
};

# endif
