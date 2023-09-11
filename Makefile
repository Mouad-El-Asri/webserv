NAME = webserv

CPP = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++11

SRCS = parse_conf/conf_parser.cpp \
	parse_conf/utils.cpp \
	parse_conf/Directives.cpp \
	main.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)

.PHONY : all clean fclean re run
