#include "Interpretter.h"


const std::vector<char> Interpretter::SPECIAL_TOKENS = {'|', '<', '>', '&'};


Interpretter::Interpretter(std::vector<std::string> command_tokens){
	this->command_tokens = command_tokens;
	this->index = 0;
}

void Interpretter::execute_command(){
	pid_t pid = fork();
	if(pid == 0){
		while(1){
			int size = 0;
			char **command = get_next_command(size);
			if(index < command_tokens.size() && this->command_tokens[this->index] == "|"){
				this->index++;
				int fd[2];  // File descriptors for pipe
				pipe(&fd[0]);  // Create pipe
				pid_t sub_pid = fork();
				if(sub_pid != 0){
					// Parent
					close(fd[0]);  // Doesn't need to read from pipe
					close(STD_OUTPUT);  // Will write to pipe instead
					dup(fd[1]);  // Put in process table
					close(fd[1]);  // No longer needed
					if(size > 0){
						execvp(command[0], command);
					}
					int sub_status;
					waitpid(sub_pid, &sub_status, 0);
					_exit(1);
				}else{
					close(fd[1]);  // Doesn't need to write to pipe
					close(STD_INPUT);  // Will read from pipe instead
					dup(fd[0]);  // Put in process table
					close(fd[0]);  // No longer needed
 				}
			}else{
				if(size > 0){
					execvp(command[0], command);
				}
				_exit(1);				
			}
		}
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
	return command;

}




