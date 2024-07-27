#include "../inc/main.hpp"

int turn_off = false;

void	ctrlc(int s) {
	if (s == 2)
		turn_off = true;
}

void	sigHandler() {
	signal(SIGINT, ctrlc);
	signal(SIGQUIT, SIG_IGN);
}

int main(int argc, char *argv[]) {

	if (argc != 3) {
		std::cout << RED << "ERROR" << RESET << ": " << ORANGE << "To run the program do: ./ircserv <PORT> <PASSWORD>" << RESET << std::endl;
		return 1;
	}

	while (1) {
		sigHandler();

		if (turn_off)
			break;
	}
	(void)argv;

	return 0;
}