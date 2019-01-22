// Main file of the project

#include "main.h"

int main(int argc, char** argv) {

	// Handle signals
	signal(SIGINT, sig_int_handler);
	signal(SIGCHLD, sig_chld_handler);

	
	std::string command_string;
	while(1){
		if(argc == 1) {
			printf("shell: ");
		}else if(argc > 1 && strcmp(argv[1],"-n") != 0) {
			printf("Improper ussage: Bad command line argument\n");
			return 1;
		}

		std::getline(std::cin, command_string);  // Get user input
		if(std::cin.eof()){  // Handle no input/ ctrl + D
			return 0;
		}else if(command_string != ""){
			std::vector<std::string> command_tokens = parse_user_input(command_string);  // Parses input
			Interpretter interpretter(command_tokens);  // Interpret the commands in the command tokens vector
			interpretter.execute_command();
		}

	}
	





	return 0;
}