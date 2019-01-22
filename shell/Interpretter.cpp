#include "Interpretter.h"


const std::vector<char> Interpretter::SPECIAL_TOKENS = {'|', '<', '>', '&', '\t'};
int parent_read_fd;

Interpretter::Interpretter(std::vector<std::string> command_tokens, int error_fd[]){
	this->command_tokens = command_tokens;
	this->index = 0;
	this->error_fd = error_fd;
}

void Interpretter::execute_command(){
	pid_t pid = fork();
	if(pid == 0){

		close(STD_ERROR);  // Going to pipe errors out instead
		close(error_fd[0]);
		dup(error_fd[1]);  // Write side of pipe for errors
		close(error_fd[1]);  // No longer needed

		while(1){
			int size = 0;
			char **command = get_next_command(size);

			// Handle input redirection
			if(index < command_tokens.size() && this->command_tokens[this->index] == "<"){
				this->index++;
				int fd = open(this->command_tokens[this->index++].c_str(), O_RDONLY);
				if(fd > 0){  // Successful open of fd
					close(STD_INPUT);  // Close stdin
					dup(fd);  // Put in process table for stdin to read from file
					close(fd);  // Close unneeded file descriptor
				}else{
					perror("");  // Output to stderr
					delete [] command;
					_exit(1);
				}
			}

			// Handle output redirection
			if(index < command_tokens.size() && this->command_tokens[this->index] == ">"){
				this->index++;
				int fd = open(this->command_tokens[this->index++].c_str(), O_WRONLY|O_CREAT, 0666);
				if(fd > 0){  // Successful open/create of fd
					close(STD_OUTPUT);  // Close stdout
					dup(fd);  // Put in process table for stdout to write to file
					close(fd);  // Close unneeded file descriptor
				}else{
					perror("");
					delete [] command;
					_exit(1);
				}
			}


			// Handle cd command
			if(size >= 2 && std::string(command[0]) == "cd"){
				delete [] command;
				_exit(1);
			}

			// Handle pipes
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
						if(execvp(command[0], command) < 0){
							perror("");
						}
					}
					int sub_status;
					waitpid(sub_pid, &sub_status, 0);
					delete [] command;  // Free memory from the getnextcommand() call

					_exit(1);
				}else{
					close(fd[1]);  // Doesn't need to write to pipe
					close(STD_INPUT);  // Will read from pipe instead
					dup(fd[0]);  // Put in process table
					close(fd[0]);  // No longer needed
					delete [] command;  // Free memory from the getnextcommand() call
 				}
			}else{
				if(size > 0){
					if(execvp(command[0], command) < 0){
						perror("");
					}
				}

				delete [] command;  // Free memory from the getnextcommand() call
				_exit(1);				
			}
		}
	}else{
		close(error_fd[1]);
		parent_read_fd = dup(error_fd[0]);
		close(error_fd[0]);

		// Handle cd command
		int size = 0;
		char **command = get_next_command(size);  // To check for cd
		if(size >= 2 && std::string(command[0]) == "cd"){
			if(command[1] == NULL){
				chdir(".");
				perror("ERROR");
			}else{
				chdir(command[1]);  // Changes directory to the specified path, MUST BE DONE IN PARENT
				perror("ERROR");
			}
		}
		delete [] command;
		int status;

		if(this->command_tokens[command_tokens.size() - 1] != "&"){
			waitpid(pid, &status, 0);
		}
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
			command = nullptr;
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
		command = nullptr;
		command = temp;  // Reassign to enlarged array			
	}
	command[size++] = NULL;  // Add NULLL at end
	return command;

}




