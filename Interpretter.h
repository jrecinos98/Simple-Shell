#ifndef INTERPRETTER_H
#define INTERPRETTER_H

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

#define STD_INPUT 0
#define STD_OUTPUT 1


class Interpretter {
	static const std::vector<char> SPECIAL_TOKENS;
public:

	// Constructor takes command tokens that were generated in main
	Interpretter(std::vector<std::string> command_tokens);
	
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


};



#endif /* INTERPRETTER_H */