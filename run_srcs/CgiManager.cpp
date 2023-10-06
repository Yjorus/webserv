# include "../inc/CgiManager.hpp"

executestuff(&error_code) {
	if (pipe(_pipe_cgi) < 0)
	{
		error_code = 500;
		return ; // maybe int?
	}
	fork (_cgi_pid);
	if (_cgi_pid == -1)
	{
		close(_pipe_cgi[0]);
		clode(_pipe_cgi[1]);
		error_code = 500;
		return ;
	}
	execve() //cgi-path, arguments, env;
}

setupEnvCgi() {
	this->_cgi_env["CONTENT_TYPE"] = ;
	this->_cgi_env["CONTENT_LENGTH"] = ;
	this->_cgi_env["HTTP_COOKIE"] = ;
	this->_cgi_env["HTTP_USER_AGENT"] = ;
	this->_cgi_env["PATH_INFO"] = ;
	this->_cgi_env["QUERY_STRING"] = ;
	this->_cgi_env["REMOTE_ADDR"] = ;
	this->_cgi_env["REMOTE_HOST"] = ;
	this->_cgi_env["REQUEST_METHOD"] = ;
	this->_cgi_env["SCRIPT_FILENAME"] = ;
	this->_cgi_env["SCRIPT_NAME"] = ;
	this->_cgi_env["SERVER_NAME"] = ;
	this->_cgi_env["SERVER_SOFTWARE"] = ;
}