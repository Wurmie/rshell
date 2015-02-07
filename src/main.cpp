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
		perror("error with fork(child) ");
		exit(1);
	}
	else if(pid == 0){		//child process running
		std::cout << "Inside child process ";
		if(execvp(argv[0],argv) == -1)
		{	
			perror("There was an error in execvp. ");
		}
		exit(1);		//kill child
	}
	else{ 				//in parent
		if(waitpid(pid,&status,0) == -1){
			perror("error with wait ");
			exit(1);
		}
	}
	//parent function here
	delete []argv;
	if(status == 0)
		return true;
	else
		return false;
}

//adds spaces before and  after every connector and takes out comments as well
void addSpaces(std::string &inputs)
{
	//find #
	size_t temp = inputs.find("#");
	
	if(temp > 0)		//first word is not #
	{
		inputs = inputs.substr(0,temp);
	}
	else if(temp == 0)	//first word is #
		inputs = " ";
	
	//find ;
	temp = 0;
	while(temp < inputs.size() && temp >= 0)
	{
		if(temp == 0)
			temp = inputs.find(";"); //will return -1 if no found
		else if(temp > 0)		//was found last time
			temp = inputs.find(";",temp+1);
		else
			break;

		if(temp != std::string::npos && temp > 0)  //; is found and not first word
		{
			inputs.replace(temp,1," ; ");
			temp = temp + 1;
		}
		else if(temp != std::string::npos && temp == 0 && (temp + 1) < inputs.size()) //; is found and is first word and not the only word
		{
			inputs.replace(temp,1,"; ");
			temp = temp + 1;
		}
		else
			break;
	}
	
	//find &&
	temp = 0;
	while(temp < inputs.size() && temp >= 0)
        {
                if(temp == 0)
                        temp = inputs.find("&&"); //will return -1 if no found
                else if(temp > 0)               //was found last time
                        temp = inputs.find("&&",temp+2);
                else
                        break;

                if(temp != std::string::npos && temp > 0)  //; is found and not first word
                {
                        inputs.replace(temp,2," && ");
                        temp = temp + 2;
                }
                else if(temp != std::string::npos && temp > 0) //; is found and is first word and not the only word
                {
                        
                }
                else
                        break;
        }

	//find ||
	temp = 0;
	while(temp < inputs.size() && temp >= 0)
        {
                if(temp == 0)
                        temp = inputs.find("||"); //will return -1 if no found
                else if(temp > 0)               //was found last time
                        temp = inputs.find("||",temp+2);
                else
                        break;

                if(temp != std::string::npos && temp > 0)  //; is found and not first word
                {
                        inputs.replace(temp,2," || ");
                        temp = temp + 2;
                }
                else if(temp != std::string::npos && temp == 0 && (temp + 1) < inputs.size()) //; is found and is first word and not the only word
                {
                        inputs.replace(temp,2,"; ");
                        temp = temp + 2;
                }
                else
                        break;
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
		std::vector<char*> chainCom;
		std::string convtStr;	
		for(size_t i = 0; i < commands.size();i++)			//loop through entire vector
		{
			convtStr = commands.at(i); //convert to string to use

			if(convtStr.compare("exit") == 0)	//exit when find "exit"
				exit(1);
			else
			{
				if(convtStr.compare(";")==0 && chainCom.size() != 0 && firstCommand)	//found connector ;
				{
					prevCommand = syscalls(chainCom);
					chainCom.clear();
					
					if(commands.size() != i+1)
					{
						i++;	
						convtStr = commands.at(i);
						if(convtStr.compare("exit") == 0)
							 exit(1);	//if next one is exit
						else
						{
							chainCom.push_back(commands[i]);
							prevCommand = syscalls(chainCom);
						}
						firstCommand = false;
						chainCom.clear();
					}						
				}
				else if(convtStr.compare("&&") == 0 && chainCom.size() != 0 && firstCommand)	//found connector &&
				{
					prevCommand = syscalls(chainCom);	//output first command		
					chainCom.clear();

					if(commands.size() != i+1)
					{
						i++;
						convtStr = commands.at(i);
						if(convtStr.compare("exit") == 0)
							exit(1);
						else if(prevCommand)
						{
							chainCom.push_back(commands[i]);
							prevCommand = syscalls(chainCom);
						}		
						else
							perror("first command was false so cannot execute ");
						firstCommand = false;
						chainCom.clear();
					}
				}
				else if(convtStr.compare("||") == 0 && chainCom.size() != 0 && firstCommand)	//found connector ||
				{
					prevCommand = syscalls(chainCom);
					chainCom.clear();

					if(commands.size() != i+1)
					{	
						i++;
						convtStr = commands.at(i);
						if(convtStr.compare("exit") == 0)
							exit(1);
						else if(!prevCommand)
						{
							chainCom.push_back(commands[i]);
							prevCommand = syscalls(chainCom);
						}
						else
							perror("first command was true so cannot execute ");
						firstCommand = false;
						chainCom.clear();
					}
				}
				else							//no connectors after
				{
					//if(convtStr.compare(";") == 0 || convtStr.compare("||") == 0 || convtStr.compare("&&") == 0)
					if(convtStr.compare(";") == 0 && commands.size() != i+1)	//if not first command and found ; and not last position
					{
						i++;
						chainCom.push_back(commands[i]);
						prevCommand = syscalls(chainCom);
						chainCom.clear();
					}
					else if(convtStr.compare("&&") == 0 && commands.size() != i+1)
					{
						i++;
						if(prevCommand)
						{
							chainCom.push_back(commands[i]);
							prevCommand = syscalls(chainCom);
							chainCom.clear();
						}
						else
							perror("first command was false so cannot execute ");
					}
					else if(convtStr.compare("||") == 0 && commands.size() != i+1)
                                        {
                                                i++;
                                                if(!prevCommand)
                                                {
                                                        chainCom.push_back(commands[i]);
                                                        prevCommand = syscalls(chainCom);
                                                        chainCom.clear();
                                                }
                                                else
                                                        perror("first command was true so cannot execute ");
                                        }

					else if(firstCommand && commands.size() == i+1)		//first word and nothing after
					{
						if((convtStr.compare(";") != 0) && (convtStr.compare("||") != 0) && (convtStr.compare("&&") != 0))
						{
							chainCom.push_back(commands[i]);
							prevCommand = syscalls(chainCom);
							break;
						}
						else
							break;
					}
					else
					{
						chainCom.push_back(commands[i]);
						if(!firstCommand)
							firstCommand = true;
					}
				}
			}
		}	
		}
	}
	return 0;
}
