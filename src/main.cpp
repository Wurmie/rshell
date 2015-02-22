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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


bool singleRedirection(std::vector<char*> command1, int whichDir)
{
	size_t newplace = 0;
        size_t find = false;

        while(newplace < command1.size() && !find)
        {
                std::string newString= command1[newplace];
                if(newString.compare(">") != 0 && newString.compare(">>") != 0 && newString.compare("<") != 0)
                        newplace++;
                else
                        find = true;
        }
	
//	for(size_t i = 0; i < newplace;i++)
//		std::cout << command1[i];

	int newFileplace = newplace;
	newFileplace++;
//	std::string  theFile = command1[newFileplace];
//	std::cout << command1[newFileplace--] << " ";

	int status;
        int argc = newplace--;                            //size of argv
        char **argv = new char*[argc];                  //make argv pointer pointer for execvp.
        for(int a = 0;a < argc;a++)                   //shove everything in command into argv
        {
                argv[a] = command1[a];
        }
        argv[argc] = '\0';
//	std:: cout << argv[newFileplace];	
//	for(size_t i = 0; argv[i] != '\0';i++)
//		std::cout << argv[i] << " ";	
        //now argv has everything in command    
        int pid = fork();
        if(pid < 0){
                perror("error with fork(child) ");
                exit(1);
        }
        else if(pid == 0){              //child process running
                //redirection in child LOL fuck me
		int place;
		int dupTwo;
		int newIn;
		int newOut;
		//Output redirection
		if(whichDir != 2)
		{
			newOut = dup(1);
			if(newOut == -1)
				perror("DUPPPPPPPP");
			if(access(command1[newFileplace],F_OK) != -1)
			{
				if(whichDir == 0)
					place = open(command1[newFileplace],O_WRONLY | O_TRUNC, 00744);
				else
					place = open(command1[newFileplace],O_WRONLY | O_APPEND, 00744);
			}
			else
				place = open(command1[newFileplace],O_WRONLY | O_CREAT, 00744);
			dupTwo = dup2(place,1);
		}
		//input redirection
		else 
		{
			newIn = dup(0);
			if(newIn == -1)
				perror("DUP IN");
			place = open(command1[newFileplace], O_WRONLY | O_RDONLY);
			dupTwo = dup2(place,1);
			
		}
		if(dupTwo == -1)
			perror("DUP2222 ERRORR");
		
                if(execvp(argv[0],argv) == -1)
                {
                        perror("There was an error in execvp1. ");
                }
		if(whichDir != 2)	
			dup2(newOut,1);
		else
			dup2(newIn,0);
		if(close(place) == -1)
			perror("error closing");
                exit(1);                //kill child
        }
        else{                           //in parent
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

bool syscalls(std::vector<char*> command)
{
	int status;
	int argc = command.size()+1;				//size of argv
	char **argv = new char*[argc];			//make argv pointer pointer for execvp.
	for(int a = 0;a < argc-1;a++)			//shove everything in command into argv
	{
		argv[a] = command[a];
	}
	//now argv has everything in command	
	int pid = fork();
	if(pid < 0){
		perror("error with fork(child) ");
		exit(1);
	}
	else if(pid == 0){		//child process running
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
		inputs = inputs.substr(0,temp);
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
	bool ConnectorFound = false;
	while(temp < inputs.size() && temp >= 0)
        {
		if(temp == 0)
			temp = inputs.find("|"); //will return -1 if no found
		else if(temp > 0 && ConnectorFound)
			temp = inputs.find("|",temp+2);
		else if(temp > 0 && !ConnectorFound)               //was found last time
			temp = inputs.find("|",temp+1);
		else
			break;

		if(inputs[temp+1] == '|')	//finds ||
		{
			if(temp != std::string::npos && temp > 0)  //| is found and not first word
			{
				inputs.replace(temp,2," || ");
				temp = temp + 2;
				ConnectorFound = true;
			}
			else if(temp != std::string::npos && temp == 0 && (temp + 1) < inputs.size()) //; is found and is first word and not the only word
			{
				inputs.replace(temp,2,"|| ");
				temp = temp + 2;
				ConnectorFound = true;
			}
			else
				break;
		}
		else //finds |
		{
			if(temp != std::string::npos && temp > 0)  //; is found and not first word
                        {
                                inputs.replace(temp,1," | ");
                                temp = temp + 1;
                        }
                        else if(temp != std::string::npos && temp == 0 && (temp + 1) < inputs.size()) //; is found and is first word and not the only word
                        {
                                inputs.replace(temp,1,"| ");
                                temp = temp + 1;
                        }
                        else
                                break;

		}
        }
	ConnectorFound = false;
	while(temp < inputs.size() && temp >= 0)
        {
                if(temp == 0)
                        temp = inputs.find(">"); //will return -1 if no found
                else if(temp > 0 && ConnectorFound)
                        temp = inputs.find(">",temp+2);
                else if(temp > 0 && !ConnectorFound)               //was found last time
                        temp = inputs.find(">",temp+1);
                else
                        break;

                if(inputs[temp+1] == '>')       //finds ||
                {
                        if(temp != std::string::npos && temp > 0)  //| is found and not first word
                        {
                                inputs.replace(temp,2," >> ");
                                temp = temp + 2;
                                ConnectorFound = true;
                        }
                        else if(temp != std::string::npos && temp == 0 && (temp + 1) < inputs.size()) //; is found and is first word and not the only word
                        {
                                inputs.replace(temp,2,">> ");
                                temp = temp + 2;
                                ConnectorFound = true;
                        }
                        else
                                break;
                }
                else //finds |
                {
                        if(temp != std::string::npos && temp > 0)  //; is found and not first word
                        {
                                inputs.replace(temp,1," > ");
                                temp = temp + 1;
                        }
                        else if(temp != std::string::npos && temp == 0 && (temp + 1) < inputs.size()) //; is found and is first word and not the only word
                        {
                                inputs.replace(temp,1,"> ");
                                temp = temp + 1;
                        }
                        else
                                break;

                }
        }
	
	while(temp < inputs.size() && temp >= 0)
	{
		if(temp == 0)
			temp = inputs.find("<"); //will return -1 if not found
		else if(temp > 0)
			temp = inputs.find("<",temp+1);
		else
			break;

		if(temp != std::string::npos && temp > 0 && inputs[temp+1] != '<') //| is found
		{
			inputs.replace(temp,1," < ");
			temp = temp + 1;
		}
		else if(temp != std::string::npos && temp == 0 && (temp + 1) < inputs.size() && inputs[temp+1] != '<')
		{
			inputs.replace(temp,1,"< ");
			temp = temp + 1;
		}       
		else
			break;
	}

}

bool check(std::string checkValue)
{
	if(checkValue.compare("&&") != 0 && checkValue.compare("||") != 0 && checkValue.compare(";") != 0)
		return true;
	else
		return false;
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
/*
		for(size_t lol = 0;lol < commands.size();lol++)
		{
			std::cout << commands[lol] << std::endl;
		}
*/
		//commands has all the words in vector of char*	
		bool prevCommand;
		bool firstCommand = true;
		bool keepGoing = true;
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
							while(check(convtStr) && keepGoing)
                                                        {
                                                                chainCom.push_back(commands[i]);
                                                                if((i+1) < commands.size())
                                                                {
                                                                        i++;
                                                                        convtStr = commands.at(i);
                                                                }
                                                                else
                                                                        keepGoing = false;
                                                        }

							prevCommand = syscalls(chainCom);
						}
						keepGoing = true;
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
							while(check(convtStr) && keepGoing)
							{
								chainCom.push_back(commands[i]);
								if((i+1) < commands.size())
								{
									i++;
									convtStr = commands.at(i);
								}
								else
									keepGoing = false;
							}
							prevCommand = syscalls(chainCom);
						}		
						else
							perror("first command was false so cannot execute ");
						firstCommand = false;
						keepGoing = true;
						chainCom.clear();
					}
				}
				else if(convtStr.compare("||") == 0 && chainCom.size() != 0 && firstCommand)	//found connector ||
				{
					std::cout << "here";
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
							while(check(convtStr) && keepGoing)
							{
								chainCom.push_back(commands[i]);
								if((i+1) < commands.size())
								{
									i++;
									convtStr = commands.at(i);
								}
								else
									keepGoing = false;
							}
							prevCommand = syscalls(chainCom);
						}
						else
							perror("first command was true so cannot execute ");
						firstCommand = false;
						keepGoing = true;
						chainCom.clear();
					}
				}
				
				else if(convtStr.compare(">") == 0 && chainCom.size() != 0 && firstCommand)
				{
					chainCom.push_back(commands[i]);
					i++;
					convtStr = commands.at(i);
					chainCom.push_back(commands[i]);
					prevCommand = singleRedirection(chainCom,0);
					firstCommand = false;
					chainCom.clear();
				}

				else if(convtStr.compare(">>") == 0 && chainCom.size() != 0 && firstCommand)
                                {
                                        chainCom.push_back(commands[i]);
                                        i++;
                                        convtStr = commands.at(i);
                                        chainCom.push_back(commands[i]);
                                        prevCommand = singleRedirection(chainCom,1);
                                        firstCommand = false;
                                        chainCom.clear();
                                }

				else							//no connectors after
				{
					if(convtStr.compare(";") == 0 && commands.size() != i+1)        //if not first command and found ; and not last position
                                        {
                                                if(firstCommand)
                                                {
                                                        perror("error in bash ");
                                                        break;
                                                }
                                                else
                                                {
                                                        i++;
							convtStr = commands[i];
							while(check(convtStr) && keepGoing)
                                                        {
                                                                chainCom.push_back(commands[i]);
                                                                if((i+1) < commands.size())
                                                                {
                                                                        i++;
                                                                        convtStr = commands.at(i);
                                                                }
                                                                else
                                                                        keepGoing = false;
                                                        }

                                                        prevCommand = syscalls(chainCom);
                                                        keepGoing = true;
							chainCom.clear();
                                                }
                                        }
                                        else if(convtStr.compare("&&") == 0 && commands.size() != i+1)
                                        {
                                                if(firstCommand)
                                                {
                                                        perror("error in bash ");
                                                        break;
                                                }
                                                else
                                                {
                                                        i++;
							convtStr = commands[i];
                                                        if(prevCommand)
                                                        {
								while(check(convtStr) && keepGoing)
                                                        	{
                                                                	chainCom.push_back(commands[i]);
                                                                	if((i+1) < commands.size())
                                                                	{
                                                                	        i++;
                                                                	        convtStr = commands.at(i);
                                                                	}
                                                                	else
                                                                	        keepGoing = false;
                                                       	 	}	

								keepGoing = true;
                                                                chainCom.clear();
                                                        }
                                                        else
                                                                perror("first command was false so cannot execute ");
                                                }
                                        }
					else if(convtStr.compare("||") == 0 && commands.size() != i+1)
                                        {
                                                if(firstCommand)
                                                {
                                                        perror("error in bash ");
                                                        break;
                                                }
                                                else
                                                {
                                                        i++;
							convtStr = commands[i];
                                                        if(!prevCommand)
                                                        {
								while(check(convtStr) && keepGoing)
                                                        	{
                                                                	chainCom.push_back(commands[i]);
                                                                	if((i+1) < commands.size())
                                                                	{
                                                                	        i++;
                                                                	        convtStr = commands.at(i);
                                                                	}
                                                                	else
                                                                 	       keepGoing = false;
                                                        	}

                                                                keepGoing = true;
								chainCom.clear();
                                                        }
                                                        else
                                                                perror("first command was true so cannot execute ");
                                                }
                                        }
					
					else if(convtStr.compare(">") == 0 && commands.size() != i+1)
					{
						chainCom.push_back(commands[i]);
						i++;
						chainCom.push_back(commands[i]);
						prevCommand = singleRedirection(chainCom,0);
						chainCom.clear();
					}

					else if(convtStr.compare(">>") == 0 && commands.size() != i+1)
                                        {
                                                chainCom.push_back(commands[i]);
                                                i++;
                                                chainCom.push_back(commands[i]);
                                                prevCommand = singleRedirection(chainCom,1);
                                                chainCom.clear();
                                        }


					else if(firstCommand && commands.size() == i+1)		//first word and nothing after
					{
						if((convtStr.compare(";") != 0) && (convtStr.compare("||") != 0) && (convtStr.compare("&&") != 0))
						{
							chainCom.push_back(commands[i]);
							prevCommand = syscalls(chainCom);
							chainCom.clear();
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
		commands.clear();	
		}
	}
	return 0;
}
