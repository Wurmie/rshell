#Rshell
##Overview
This is a copy of the original BASH terminal. Along with working like an original BASH, it can also run multiple commands in one line with ; and && and || connectors as well as # for comments. ; will always run the next command; || will only run if the command before that failed; && will only run if the command before it succeeded.

For ls, it basically mirrors the BASH terminal ls for -R, -a, -l and its paths. 

##How to use
Run the program in a linux terminal. This program will not guarantee to run properly even in a linux terminal.
Even though echo and ls are different commands, when testing, they are the same thing.

There is a scripts in test folder that I used when testing for test cases from Mevens.

To run the program for RShell(hw0,hw2,hw3):

``$git clone https://github.com/Wurmie/rshell.git``

``$cd rshell``

``$git checkout hw2``

``$make``

``$bin/rshell``


To run the program for ls:

``$git clone https://github.com/wurmie/rshell.git``

``$cd rshell``

``$git checkout hw2``

``$make``

``$bin/ls``

For ls, you need to add ls after bin/ls. ex: bin/ls ls (flags here)

To run cp.cpp program:

``$git clone https://github.com/wurmie/rshell.git``

``$cd rshell``

``$git checkout hw2``

``$make``

``$bin/cp``

You might need to go into edit mode. I backspaced alot.
##Prerequisites
Need to run the program:

	* Linux

	* g++

	* vim

##Bugs/Limitations/Issues
rshell.cpp

	* tab does not work

	* piping does not work
	
	* ^Z does not work

ls.cpp

	* when all 3 flags are passed with file, does not output everything
	
	* R sometimes does not work properly
	 
	* None of the special characters work properly.
	 
	* multiple /'s do not work properly

#Comments
	to save space, I hard commented each test case. 
	
	If I try to comment in bash, it wastes too much space and too hard to read. 
	
	Sorry. And sorry again for the backspaces.
