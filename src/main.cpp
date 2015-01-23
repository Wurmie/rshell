#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


bool syscalls(std::vector<char*> command)
{
	int argc = command.size()+1;				//size of argv
	char **argv = new char*[argc];			//make argv pointer pointer for execvp.
	for(int a = 0;a < argc-1;a++)			//shove everything in command into argv
	{
		argv[a] = command[a];
	}
	argv[argc-1] = '\0';	
	//now argv has everything in command	
	int pid = fork();
	if(pid < 0){
		perror("error with fork(child)");
		exit(1);
	}
	else if(pid == 0){		//child process running
		std::cout << "Inside child process ";
		if(-1 == execvp(argv[0],argv))
			perror("There was an error in execvp. ");
		exit(1);		//kill child
	}
	else{ 				//in parent
		if(wait(0) == -1){
			perror("error with wait");
			exit(1);
		}
	}
	//parent function here
}

//adds spaces after every connector and takes out comments as well
void addSpaces(std::string &inputs)
{
	for(int x = 0;x < inputs.size();++x)
	{
		
	}
}
int main()
{
	std::string input;		//string value to get input
	char hostname[100];		//get the hostname from terminal
	char username[100];		//get username from terminal
	gethostname(hostname,100);
	getlogin_r(username,100);
	while(1)			//run until exit(1)
	{
		std::cout << username << "@" << hostname << "$"; //output beg
		getline(std::cin,input);			//get input
		
		addSpaces(input);
	
		char *tok = strtok(const_cast<char*>(input.c_str())," "); //will break the words if not a space
		std::vector<char*> commands;				     //vector to store the commands
		while(tok != NULL)					     //loop till end
		{
			commands.push_back(tok);
			tok = strtok(NULL," ");
		}

		for(int i = 0; i < commands.size();i++)			//loop through entire vector
		{
			bool prevCommand = false;
			std::vector<char*> chainCom;			//vector with a chain of commands for argv. String too hard
			std::string convtStr = commands.at(i);			//convert to string to use
			if(convtStr.find("exit") != std::string::npos)	//exit when find "exit"
				exit(0);
			else
			{
				if(convtStr.find(";") != std::string::npos)	//found connector ;
				{
					int place = convtStr.find(";");
					if(place > 0){
						convtStr = convtStr.substr(0,place);
						chainCom.push_back(commands[i]);
					}
						
					prevCommand = syscalls(chainCom);
					chainCom.clear();	
				}
				else if(convtStr.find("&&") != std::string::npos)	//found connector &&
				{
					
				}
				else if(convtStr.find("||") != std::string::npos)	//found connector ||
				{
			
				}
				else if(convtStr.find("#") != std::string::npos)	//its all comments
				{
					
				}
				else							//no connectors
				{
					chainCom.push_back(commands[i]);			//add commands until ;&| found
					prevCommand = syscalls(chainCom);
				}	
			}
		}
	}
	return 0;
}
