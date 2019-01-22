#ifndef INTERPRETTER_H
#define INTERPRETTER_H

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include "fd.h"

#define STD_INPUT 0
#define STD_OUTPUT 1
#define STD_ERROR 2
#define CHILD_ERRORS 10

#define BUFSIZE 2048

class Interpretter {
	static const std::vector<char> SPECIAL_TOKENS;

public:

	// Constructor takes command tokens that were generated in main
	Interpretter(std::vector<std::string> command_tokens, int error_fd[]);
	
	// Executes commands present in command tokens
	void execute_command();

	// Determines if supplied character is a special token
	bool is_special_token(std::string s);

	// Generates the command/arguments to be passed to exec
	char** get_next_command(int& size);

private:
	std::vector<std::string> command_tokens;
	int num_children;
	unsigned int index;
	int *error_fd;

};



#endif /* INTERPRETTER_H */