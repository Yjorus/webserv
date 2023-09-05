#ifndef ALLHEADERS_HPP
# define ALLHEADERS_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <cstddef>
# include <sstream>
# include <cstring>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <exception>
# include <stdexcept>
# include <vector>
# include <map>

int	checkPath(std::string path, int flag);
int checkFile(std::string path);
unsigned long my_stoul(std::string str);

#endif