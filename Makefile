CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ircserv

FILES =	utils

SRCS =	$(FILES:=.cpp) main.cpp

HEADERS =	

all: $(NAME)

$(NAME) : $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(NAME) -gdwarf-4

clean:

fclean: clean
	rm $(NAME)

re: fclean all

.PHONY: all clean fclean re