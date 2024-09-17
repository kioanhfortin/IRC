NAME = ircserv
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = main.cpp server.cpp parsing.cpp client.cpp channel.cpp cmdNick.cpp cmdUser.cpp cmdInvite.cpp \
	utils.cpp cmdJoin.cpp ping.cpp cmdPrivMsg.cpp cmdTopic.cpp cmdKick.cpp cmdMode.cpp cmdPart.cpp
OBJS = $(SRCS:.cpp=.o)

RM = rm -f

all :	$(NAME)

$(NAME) :	$(OBJS)
		$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all
