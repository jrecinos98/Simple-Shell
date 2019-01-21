all:    #target names
	g++ -static -g -std=c++11 -Wall -o simple_shell *.cpp
clean :
	\rm -fr simple_shell
