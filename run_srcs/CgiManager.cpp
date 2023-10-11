# include "../inc/CgiManager.hpp"

CgiManager::CgiManager(Request &request): _body(request.getBody())

executestuff(&error_code) {
	if (pipe(_pipe_cgi) < 0) {
		error_code = 500;
		return ; // maybe int?
	}
	this->_cgi_pid = fork()
	if (this->_cgi_pid == -1) {
		close(_pipe_cgi[0]);
		clode(_pipe_cgi[1]);
		error_code = 500;
		return ;
	}
	else if (!this->_cgi_pid) {

	}
	else {
		waitpid(-1, NULL, 0);
	}
	execve() //cgi-path, arguments, env;
}

char	**CgiManager::mapToCStrArray() {
	char	**env = new char *[this->_cgi_env.size() + 1];
	int a = 0;
	for (std::map<std::string, std::string>::const_iterator b = this->_cgi_env.begin(); b != this->_cgi_env.end(); b++) {
		std::string	element = b->first + "=" + b->second;
		env[a] = new char[element.size() + 1];
		env[a] = strcpy(env[j], (const char*)element.c_str());
		a++;
	}
	env[a] = NULL;
	return (env);
}

setupEnvCgi(Request &request) {

	this->_cgi_env["AUTH_TYPE"] = hold["Basic"] ;
	this->_cgi_env["CONTENT_LENGTH"] = request.getHeader("Content-Length"); //stringstream
	this->_cgi_env["CONTENT_TYPE"] = request.getHeader("Content-Type");
	this->_cgi_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_cgi_env["PATH_INFO"] = ;
	this->_cgi_env["PATH_TRANSLATED"] = ;
	this->_cgi_env["QUERY_STRING"] = request.getQuery();
	this->_cgi_env["REMOTE_ADDR"] = request.getAddress();
	this->_cgi_env["REMOTE_HOST"] = request.getServerName();
	this->_cgi_env["REMOTE_IDENT"] = ;
	this->_cgi_env["REMOTE_USER"] = ;
	this->_cgi_env["REQUEST_METHOD"] = request.getMethod();
	this->_cgi_env["SCRIPT_NAME"] = ;
	this->_cgi_env["SERVER_NAME"] = request.getServerName();
	this->_cgi_env["SERVER_PORT"] = request.getPort();
	this->_cgi_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_cgi_env["SERVER_SOFTWARE"] = ;
}

//  meta-variable-name = "AUTH_TYPE" | "CONTENT_LENGTH" |
//                            "CONTENT_TYPE" | "GATEWAY_INTERFACE" |
//                            "PATH_INFO" | "PATH_TRANSLATED" |
//                            "QUERY_STRING" | "REMOTE_ADDR" |
//                            "REMOTE_HOST" | "REMOTE_IDENT" |
//                            "REMOTE_USER" | "REQUEST_METHOD" |
//                            "SCRIPT_NAME" | "SERVER_NAME" |
//                            "SERVER_PORT" | "SERVER_PROTOCOL" |
//                            "SERVER_SOFTWARE" | scheme |
//                            protocol-var-name | extension-var-name
//       protocol-var-name  = ( protocol | scheme ) "_" var-name
//       scheme             = alpha *( alpha | digit | "+" | "-" | "." )
//       var-name           = token
//       extension-var-name = token
// source : rfc3875