/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjorus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 13:30:07 by yjorus            #+#    #+#             */
/*   Updated: 2023/10/24 13:30:09 by yjorus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/CgiManager.hpp"

CgiManager::CgiManager() {
	this->_cgi_path = "";
	this->_cgi_pid = -1;
	this->_exit_code = 0;
	this->_env = NULL;
	this->_av = NULL;
	this->_timeout = 0;
}

CgiManager::~CgiManager() {
	clearCgi();
}


void	CgiManager::executeCgi(int &code) {
	if (this->_av == NULL || this->_av[0] == NULL || this->_av[1] == NULL) {
		code = 500;
		return ;
	}
	if (pipe(_pipe_cgi_in) < 0) {
		code = 500;
		return ;
	}
	if (pipe(_pipe_cgi_out) < 0) {
		close(_pipe_cgi_in[0]);
		close(_pipe_cgi_in[1]);
		code = 500;
		return ;
	}
	this->_cgi_pid = fork();
	if (this->_cgi_pid == 0) {
		dup2(_pipe_cgi_in[0], STDIN_FILENO);
		dup2(_pipe_cgi_out[1], STDOUT_FILENO);
		close(_pipe_cgi_in[0]);
		close(_pipe_cgi_in[1]);
		close(_pipe_cgi_out[0]);
		close(_pipe_cgi_out[1]);
		this->_exit_code = execve(this->_av[0], this->_av, this->_env);
		exit(this->_exit_code);
	}
	else if (this->_cgi_pid < 0) {
		code = 500;
	}
	else {
		this->_timeout = time(NULL) + 1;
	}
}

char	**CgiManager::mapToCStrArray() {
	char	**env = new char *[this->_cgi_env.size() + 1];
	int a = 0;
	for (std::map<std::string, std::string>::const_iterator b = this->_cgi_env.begin(); b != this->_cgi_env.end(); b++) {
		std::string	element = b->first + "=" + b->second;
		env[a] = new char[element.size() + 1];
		env[a] = strcpy(env[a], (const char*)element.c_str());
		a++;
	}
	env[a] = NULL;
	return (env);
}

int		CgiManager::findstart(std::string path, std::string key) {
	if (path.empty())
		return (-1);
	size_t a = path.find(key);
	if (a == std::string::npos)
		return (-1);
	return (a);
}

void	CgiManager::setupEnvCgi(Request &request, std::string extension, Location location) {

	int a;
	std::string path;
	if (!location.getCgiMap().count(extension)) 
		return ;
	else
		path = location.getCgiMap()[extension];
	this->_cgi_env["AUTH_TYPE"] = "Basic";
	this->_cgi_env["CONTENT_LENGTH"] = request.getHeader("Content-Length");
	this->_cgi_env["CONTENT_TYPE"] = request.getHeader("Content-Type");
	this->_cgi_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	a = findstart(this->_cgi_path, "cgi-bin/");
	// this->_cgi_env["PATH_INFO"] = ;
	// this->_cgi_env["PATH_TRANSLATED"] = ;
	this->_cgi_env["QUERY_STRING"] = request.getQuery();
	this->_cgi_env["REMOTE_ADDR"] = request.getAddress();
	this->_cgi_env["REMOTE_HOST"] = request.getServerName();
	// this->_cgi_env["REMOTE_IDENT"] = ;
	// this->_cgi_env["REMOTE_USER"] = ;
	this->_cgi_env["REQUEST_METHOD"] = request.getMethod();
	this->_cgi_env["SCRIPT_NAME"] = this->_cgi_path;
	this->_cgi_env["SCRIPT_FILENAME"] = ((a < 0 || (size_t)(a + 8) > this->_cgi_path.size()) ? "" : this->_cgi_path.substr(a + 8, this->_cgi_path.size()));
	this->_cgi_env["SERVER_NAME"] = request.getServerName();
	this->_cgi_env["SERVER_PORT"] = request.getPort();
	this->_cgi_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_cgi_env["SERVER_SOFTWARE"] = "LulzSec Server";

	std::map<std::string, std::string> request_headers = request.getHeaders();
	for(std::map<std::string, std::string>::iterator it = request_headers.begin(); it != request_headers.end(); ++it)
	{
		std::string name = it->first;
		std::transform(name.begin(), name.end(), name.begin(), ::toupper);
		std::string key = "HTTP_" + name;
		this->_cgi_env[key] = it->second;
	}

	this->_env = mapToCStrArray();
	this->_av = new char *[3];
	this->_av[0] = new char[path.size() + 1];
	this->_av[0] = strcpy(this->_av[0], (const char*)path.c_str());
	this->_av[1] = new char[this->_cgi_path.size() + 1];
	this->_av[1] = strcpy(this->_av[1], (const char*)this->_cgi_path.c_str());
	this->_av[2] = NULL;
}

void	CgiManager::setPath(std::string &path) {
	this->_cgi_path = path;
}

std::string	CgiManager::getPath() {
	return (this->_cgi_path);
}

void	CgiManager::clearCgi() {
	this->_cgi_path.clear();
	if (this->_env) {
		for (int a = 0; this->_env[a]; a++)
			delete [] this->_env[a];
		delete [] this->_env;
	}
	if (this->_av) {
		for (int a = 0; this->_av[a]; a++)
			delete [] this->_av[a];
		delete [] this->_av;
	}
	this->_env = NULL;
	this->_av = NULL;
	this->_cgi_pid = -1;
	this->_exit_code = 0;
	this->_cgi_env.clear();
}

void	CgiManager::checkTimeout() {
	if (time(NULL) > this->_timeout) {
		kill(this->_cgi_pid, SIGKILL);
		this->_exit_code = 1;
	}

}

pid_t		CgiManager::getPid() {
	return (this->_cgi_pid);
}
