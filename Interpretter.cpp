#include "Interpretter.h"


const std::vector<char> Interpretter::SPECIAL_TOKENS = {'|', '<', '>', '&'};


Interpretter::Interpretter(std::vector<std::string> command_tokens){
	this->command_tokens = command_tokens;
}

void Interpretter::execute_command(){
	pid_t pid = fork();
	if(pid == 0){
		int size = 0;
		char **command = get_next_command(size);
		execvp(command[0], command);
		exit(1);
	}else{
		int status;
		waitpid(pid, &status, 0);
	}
}

bool Interpretter::is_special_token(std::string s){
	for(unsigned int i = 0; i < SPECIAL_TOKENS.size(); ++i){
		std::string str(1, SPECIAL_TOKENS[i]);
		if(s == str){  // Checks if string is a special token
			return 1;
		}
	}
	return 0;
}

char** Interpretter::get_next_command(int& size){
	size = 0;
	char **command = new char*[1];
	int max_size = 1;
	unsigned int index = 0;
	while(index < command_tokens.size() && !is_special_token(this->command_tokens[index])){
		if(size >= max_size){  // If there is room add the new token to array
			char **temp = new char*[size + 1];
			max_size++;  // Update max size
			std::copy(command, command + size, temp);  // Copy command to temp
			delete [] command;  // Free old memory
			command = temp;  // Reassign to enlarged array			
		}
		char *token = const_cast<char*> ( this->command_tokens[index].c_str() );
		command[size++] = token; // Read from front of vector
		index++;	
	}

	if(size >= max_size){  // Adding in the NULL at end
		char **temp = new char*[size + 1];
		max_size++;  // Update max size
		std::copy(command, command + size, temp);  // Copy command to temp
		delete [] command;  // Free old memory
		command = temp;  // Reassign to enlarged array			
	}
	command[size++] = NULL;  // Add NULLL at end

	command_tokens.erase(command_tokens.begin(), command_tokens.begin() + index);

	return command;

}




