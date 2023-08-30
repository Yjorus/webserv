SRCS = main.cpp Webserv.cpp
CC = c++
CPPFLAGS = -Werror -Wall -Wextra -std=c++98
OBJS = $(SRCS:.cpp=.o)
RM = rm -rf
NAME = webserv
INCLUDES = AllHeaders.hpp Webserv.hpp

all: ${NAME}

${NAME}: ${OBJS} ${INCLUDES}
			${CC} ${CPPFLAGS} ${OBJS} -o ${NAME}

clean:
		${RM} ${OBJS}

fclean: clean
		${RM} ${NAME}

re: fclean ${NAME}

.PHONY: all clean fclean re