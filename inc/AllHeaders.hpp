#ifndef ALLHEADERS_HPP
# define ALLHEADERS_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <cstddef>
# include <cstdlib>
# include <sstream>
# include <cstring>
# include <cctype>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <exception>
# include <stdexcept>
# include <vector>
# include <map>
# include <algorithm>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <sys/select.h>
# include <fcntl.h>
# include <sys/time.h>
# include <ctime>
# include <signal.h>

# include <errno.h>
# include <stdio.h>
# include <dirent.h>

int				checkPath(std::string path, int flag);
int				checkFile(std::string path);
unsigned long	my_stoul(std::string str);
std::string		statusCodes(int code);
int				my_stoi(std::string str);
int				checkPathAndFile(std::string path, std::string file);
int 			myInetAton(const char *cp, struct in_addr *addr);
std::string		to_String(int nbr);
std::string		readFile(std::ifstream &file);

#endif