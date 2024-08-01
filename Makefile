# -*- MakeFile -*-

NAME = ircserv

CXX = @c++

CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98

SRC = main.cpp checks.cpp server.cpp

SRCS = $(addprefix src/, $(SRC))

OBJ_DIR = obj

OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:%.cpp=%.o))

# Reset
Color_Off='\033[0m'       # Text Reset

# High Intensity
IBlack='\033[0;90m'       # Black
IRed='\033[0;91m'         # Red
IGreen='\033[0;92m'       # Green
IYellow='\033[0;93m'      # Yellow
IBlue='\033[0;94m'        # Blue
IPurple='\033[0;95m'      # Purple
ICyan='\033[0;96m'        # Cyan
IWhite='\033[0;97m'       # White

MSG1 = @echo ${IGreen}"Compiled Successfully ✔︎"${Color_Off}
MSG2 = @echo ${IYellow}"Cleaned Successfully ✔︎"${Color_Off}
MSG3 = @echo ${ICyan}"Cleaned ${NAME} Successfully ✔︎"${Color_Off}
HOWTO = @echo ${IRed}"To run the program do: ./${NAME} <PORT> <PASSWORD>"${Color_Off}

CTRL_L = clear -x # This is the same as CTRL + L cleaning the terminal without deleting everything just pussing up

all: $(NAME)

$(NAME): $(OBJS)
	@$(CTRL_L)
	@$(CXX) $(CXXFLAGS) $(SRCS) -o $(NAME)
	$(MSG1)
	${HOWTO}

$(OBJ_DIR)/%.o: $(SRCS)
	@mkdir -p $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

a: all
	@$(CTRL_L)

clean:
	@$(CTRL_L)
	@/bin/rm -rf $(NAME) ${OBJ_DIR}
	$(MSG2)

fclean: clean
	@/bin/rm -rf $(NAME) ${OBJ_DIR}
	$(MSG3)

re: fclean all