#Rshell
##Overview
This is a copy of the original BASH terminal. Along with working like an original BASH, it can also run multiple commands in one line with ; and && and || connectors as well as # for comments. ; will always run the next command; || will only run if the command before that failed; && will only run if the command before it succeeded.

For ls, it basically mirrors the BASH terminal ls for -R, -a, -l and its paths

##How to use
Run the program in a linux terminal. This program will not guarantee to run properly even in a linux terminal.
Even though echo and ls are different commands, when testing, they are the same thing.

To run the program for RShell:

``$git clone https://github.com/Wurmie/rshell.git``

``$cd rshell``

``$git checkout hw0``

``$make``

``$bin/rshell``


To run the program for ls:

``$git clone https://github.com/wurmie/rshell.git``

``$cd rshell``

``$git checkout hw1``

``$make``

``$bin/ls``

For ls, you need to add ls after bin/ls. ex: bin/ls ls (flags here)

You might need to go into edit mode. I backspaced alot.
##Prerequisites
Need to run the program:

	* Linux

	* g++

	* vim

##Bugs/Limitations/Issues
rshell.cpp
	* # do not work when appended to other characters
	* tab does not work sometimes
	* if connectors are by itself, will not work

ls.cpp

	* when all 3 flags are passed with file, does not output everything
	
	* R sometimes does not work properly
	 
	* None of the special characters work properly.
	 
	* multiple /'s do not work properly
