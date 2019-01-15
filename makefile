all:    #target names
	g++ -g -Wall -o simple_shell *.cpp
clean :
	\rm -fr simple_shell
