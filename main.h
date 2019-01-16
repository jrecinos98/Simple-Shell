#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>

#include "Interpretter.h"

// Parses the string the user enters and returns it as a vector of tokens
std::vector<std::string> parse_user_input(std::string& command_string) {
	std::vector<std::string> command_tokens;
	std::stringstream stringStream(command_string);
	std::string line;
	while(std::getline(stringStream, line)) {  // Get stringstream as string line
	    std::size_t prev = 0, pos;
	    while ((pos = line.find_first_of(" |&<>", prev)) != std::string::npos) {  // Finds the first of the delimiters
	        if (pos > prev){  // If position i
	            command_tokens.push_back(line.substr(prev, pos-prev));
	        }
	        prev = pos+1;
			if(line[pos] != ' '){  // Push token after the command string to preserve order
				std::string temp_token = "";
				temp_token += line[pos];
			 	command_tokens.push_back(temp_token);  // Pushes the token if it is not a space
			}
	    }
	    if (prev < line.length()){
	        command_tokens.push_back(line.substr(prev, std::string::npos));
	    }
	}

	return command_tokens;
}








#endif /* MAIN_H */