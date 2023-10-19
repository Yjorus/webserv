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

void	CgiManager::initCGI(Request &request, const std::vector<Location>::iterator loc)
{
	std::string	path;
	std::string	ext;

	ext = this->_loc_cgi.substr(this->_loc_cgi.find("."));
	//Maybe need to check for a error here
	const std::map<std::string, std::string> test;
	test = loc->getCgiMap(); //Idk how to fix this stuff
	path = test[ext];
	setupEnvCgi(request);
	this->_cp_env = mapToCStrArray();
	this->_argv = (char **)malloc(sizeof (char *) * 3);
	this->_argv[0] = strdup(path.c_str());
	this->_argv[1] = strdup(this->_loc_cgi.c_str());
	this->_argv[2] = NULL;
}

void	CgiManager::exec()
{
	int				file;
	int				fd[2];
	int				pid;
	int				status;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		file = open("./tmpfile.txt", O_RDWR | O_TRUNC | O_CREAT, 0444); //Read only
		// tmp = open("./", O_RDONLY);
		// ft_error(file, "First File");
		ft_error(dup2(file, STDIN_FILENO), "First Dup");
		ft_error(dup2(fd[1], STDOUT_FILENO), "Second Dup"); //Where to send the result, is it correct
		close(fd[0]);
		execve("/usr/bin/python3" , (char * const *)"../rootdir/action_page.py", (char * const *)"/bin/bash");
		close(file); //This execute but dont send the content to anywhere, or dont execute
	}
	waitpid(pid, &status, 0); //Parent will wait for the children
}

void	ft_execve(char *argv, char **envp) // is it needed ? or can enter the Cgi functiosn ith less
{
	char	*path;
	char	**cmd;

	cmd = ft_split(argv, ' ');
	if (cmd == NULL)
		ft_error(-1, "Split");
	path = find_cmdpath(cmd, envp);
	execve(path, cmd, envp);
}

char	**CgiManager::mapToCStrArray()
{
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

void CgiManager::setupEnvCgi(Request &request)
{
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

// static char	*find_cmdpath(char **cmd, char **envp)
// {
// 	char	**cmdpath;
// 	int		row;

// 	row = 0;
// 	cmdpath = find_usrpath(cmd, envp);
// 	while (access(cmdpath[row], X_OK) != 0 && cmdpath[row])
// 		row++;
// 	if (!cmdpath[row])
// 	{
// 		ft_free(cmd);
// 		exit(1);
// 	}
// 	return (cmdpath[row]);
// }

// void	ft_execve(char *argv, char **envp) // is it needed ?
// {
// 	char	*path;
// 	char	**cmd;

// 	cmd = ft_split(argv, ' ');
// 	if (cmd == NULL)
// 		ft_error(-1, "Split");
// 	path = find_cmdpath(cmd, envp);
// 	execve(path, cmd, envp);
// }




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