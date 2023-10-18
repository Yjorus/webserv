SRCS = main.cpp parsing_srcs/Webserv.cpp parsing_srcs/Server.cpp parsing_srcs/Location.cpp parsing_srcs/utils.cpp run_srcs/RunServer.cpp \
		run_srcs/Client.cpp run_srcs/Request.cpp run_srcs/Response.cpp
CC = c++
CPPFLAGS = -Werror -Wall -Wextra -std=c++98 -g
OBJS = $(SRCS:.cpp=.o)
RM = rm -rf
NAME = webserv
INCLUDES = inc/AllHeaders.hpp inc/Webserv.hpp inc/Server.hpp inc/Location.hpp inc/RunServer.hpp inc/Client.hpp inc/Request.hpp inc/Response.hpp

all: ${NAME}

${NAME}: ${OBJS} ${INCLUDES}
			${CC} ${CPPFLAGS} ${OBJS} -o ${NAME}

clean:
		${RM} ${OBJS}

fclean: clean
		${RM} ${NAME}

re: fclean ${NAME}

.PHONY: all clean fclean re