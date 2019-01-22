// Main file of the project

#include "main.h"

int main(int argc, char** argv) {
	file_desc_global = -1;  // Init to -1

	// Handle signals
	signal(SIGINT, sig_int_handler);
	signal(SIGCHLD, sig_chld_handler);

	std::string command_string;

	while(1){
		int error_fd[2];
		pipe(error_fd);
		close(file_desc_global);
		file_desc_global = dup(error_fd[0]);  // Replace fd so that can listen for errors
		close(error_fd[0]);  // No longer needed

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
			Interpretter interpretter(command_tokens, error_fd[1]);  // Interpret the commands in the command tokens vector
			interpretter.execute_command();
			close(error_fd[1]);  // Close write side

		}

	}

	return 0;
}