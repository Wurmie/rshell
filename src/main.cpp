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
	int status;
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
		if( execvp(argv[0],argv) == -1)
		{	
			perror("There was an error in execvp. ");
		}
		exit(1);		//kill child
	}
	else{ 				//in parent
		if(waitpid(pid,&status,0) == -1){
			perror("error with wait");
			exit(1);
		}
	}
	//parent function here
	if(status == 0)
		return true;
	else
		return false;
}

//adds spaces before and  after every connector and takes out comments as well
void addSpaces(std::string &inputs)
{
	//find #
	size_t x = 0;
	size_t temp = inputs.find("#");
	if(temp > 1)
	{
		inputs = inputs.substr(0,temp);
	}
	else if(temp == 1)
		inputs = " ";
	//find ;
	temp = 0;
	x = 0;
	while(temp < inputs.size())
	{
		if(temp == 0)
			temp = inputs.find(";");
		else
			temp = inputs.find(";",temp+1,1);

		if(temp != std::string::npos && x!=temp )
		{
			inputs.replace(temp,1," ; ");
			x = temp+1;
		}
	}
	
	//find &&
	temp = 0;
	x = 0;
	while(temp < inputs.size())
	{
		if(temp == 0)
			temp = inputs.find("&&");
                else
                        temp = inputs.find("&&",temp+2,2);

                if(temp != std::string::npos && x!=temp)
                {
                        inputs.replace(temp,2," && ");
                        x = temp+2;
                }
        }
	//find ||
	temp = 0;
        x = 0;
        while(temp < inputs.size())
        {
                if(temp == 0)
                        temp = inputs.find("||");
                else
                        temp = inputs.find("||",temp+2,2);

                if(temp != std::string::npos && x!=temp)
                {
                        inputs.replace(temp,2," || ");
                        x = temp+2;
                }
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
		if(input.size() != 0)
		{
		char *tok = strtok(const_cast<char*>(input.c_str())," "); //will break the words if not a space
		std::vector<char*> commands;				     //vector to store the commands
		while(tok != NULL)					     //loop till end
		{
			commands.push_back(tok);
			tok = strtok(NULL," ");
		}
		
		//commands has all the words in vector of char*

		bool prevCommand;
		bool firstCommand = true;
		int Connector = 0;
		std::vector<char*> chainCom;	
		for(size_t i = 0; i < commands.size();i++)			//loop through entire vector
		{
			std::string convtStr = commands.at(i); //convert to string to use
			if(convtStr.compare("exit") == 0)	//exit when find "exit"
				exit(1);
			else
			{
				if(convtStr.compare(";")==0 && chainCom.size() != 0)	//found connector ;
				{
					prevCommand = syscalls(chainCom);
					firstCommand = false;
					chainCom.clear();						
				}
				else if(convtStr.compare("&&") == 0 && chainCom.size() != 0)	//found connector &&
				{
					if(firstCommand == true)
					{
						prevCommand = syscalls(chainCom);
						firstCommand = false;
					}
					else if(prevCommand)
						prevCommand = syscalls(chainCom);
					else
						perror("first command was false so cannot execute");
					Connector = 1;
					chainCom.clear();
				}
				else if(convtStr.compare("||") == 0 && chainCom.size() != 0)	//found connector ||
				{
					if(firstCommand == true){
						prevCommand = syscalls(chainCom);
						firstCommand = false;
					}
					else if(!prevCommand){
						prevCommand = syscalls(chainCom);
					}
					else
						perror("first command was true so cannot execute");
					Connector = 2;
					chainCom.clear();
				}
				else							//no connectors
				{
					if((commands.size() == i+1) && !firstCommand)
					{
						if(Connector == 1 && !prevCommand)
							perror("first command was false so cannot execute");
						else if(Connector == 1 && prevCommand)
							prevCommand = syscalls(chainCom);
						else if(Connector== 2 && !prevCommand)
							prevCommand = syscalls(chainCom);
						else if(Connector == 2 && prevCommand)
							perror("first command was true so cannot execute");
						else if(Connector == 3)
							prevCommand = syscalls(chainCom);
						else
							i = commands.size()+1;
					}		
					else
						chainCom.push_back(commands[i]);
				}	
			}
		}
		}
	}
	return 0;
}
