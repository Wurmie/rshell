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

//FUCK IT. NOT PASSING IN MORE VARIABLES.
int child = 0;
int parent = -1;

void getPATH(char** path)
{
        std::string pathing = "-";
        char *thePath = getenv("PATH");
        if(thePath == NULL)
        {
                perror("error getting path");
                exit(1);
        }
        //split the path up into seperating :'s using tokenizing
        else
        {
                char *tok = strtok(thePath,":");

                std::vector<std::string> commandPaths;

                while(tok != NULL)
                {
                        commandPaths.push_back(tok);
                        tok = strtok(NULL,":");
                }

                //then get the path
                for(size_t i = 0;i < commandPaths.size(); i++)
                {
                        pathing = commandPaths.at(i) + "/" + path[0];
                        execv(pathing.c_str(), path);
                }
                perror("Error in EXECV instead of EXECVP");
                exit(1);
        }
}

bool doubleRedirection(std::vector<char*> command2, char* outputFile,char* inputFile,int whichDir)
{
	int status;
	std::string makeComString = command2[0];

        if(makeComString.compare("cd") == 0)
        {
                //nothing after cd LOL
                if(command2.size() < 2)
                {
                        //by itself. we dont have to implement the home cd function. fuck yeah
                        std::cout << "DONT HAVE TO IMPLEMENT THIS!!!!!" << std::endl;
                        status = -1;
                }
                else
                {
                        //make next word a string
                        makeComString = command2[1];
                        if(chdir(command2[1]) == -1)
                        {
                                perror("CD ERROR HAHA");
                                status = -1;
                        }
                        else
                                status = 0;
                }
                return status;
        }

	int argc = command2.size()+1;
	char **argv = new char*[argc];

	for(int a = 0;a < argc-1;a++)
		argv[a] = command2[a];

	argv[argc-1] = '\0';
	
	
	int pid = fork();
	child = pid;
	if(pid < 0){
		perror("error with fork(third child) ");
		exit(1);
	}
	else if(pid == 0){
		int place;
		int dupTwo = -1;
		int newIn;
		int newOut;
	
		//output redirection	
		
			newOut = dup(1);
			if(newOut == -1)
				perror("DUP AGAINN");
			if(access(outputFile,F_OK) != -1)
			{
				if(whichDir == 0)
					place = open(outputFile,O_WRONLY | O_TRUNC, 00744);
				else
					place = open(outputFile,O_WRONLY | O_APPEND, 00744);
			}
			else
				place = open(outputFile,O_WRONLY | O_CREAT, 00744);
			dupTwo = dup2(place,1);
		
		//input redirection
		
			newIn = dup(0);
			if(newIn == -1)
				perror("DUP IN AGAIN");
			if(access(inputFile,F_OK) != -1)
				place = open(inputFile, O_RDONLY);
			else
				perror("cannot access files again");

			dupTwo = dup2(place,0);
		
		
		if(dupTwo == -1)
			perror("DUP2 Error");

		getPATH(argv);
		if(whichDir != 2)
			dup2(newOut,1);
		else
			dup2(newIn,0);
		if(close(place) == -1)
			perror("error closing again");
		exit(1);	
	}
	else{
		if(waitpid(pid,&status,0) == -1){
			perror("error with wait again");
			exit(1);
		}
	}
	
	delete []argv;
	if(status == 0)
		return true;
	else
		return false;
}

bool singleRedirection(std::vector<char*> command1, int whichDir)
{
	int status;
	std::string makeComString = command1[0];

        if(makeComString.compare("cd") == 0)
        {
                //nothing after cd LOL
                if(command1.size() < 2)
                {
                        //by itself. we dont have to implement the home cd function. fuck yeah
                        std::cout << "DONT HAVE TO IMPLEMENT THIS!!!!!" << std::endl;
                        status = -1;
                }
                else
                {
                        //make next word a string
                        makeComString = command1[1];
                        if(chdir(command1[1]) == -1)
                        {
                                perror("CD ERROR HAHA");
                                status = -1;
                        }
                        else
                                status = 0;
                }
                return status;
        }

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
	child = pid;
        if(pid < 0){
                perror("error with fork(child) ");
                exit(1);
        }
        else if(pid == 0){              //child process running
                //redirection in child LOL fuck me
		int place;
		int dupTwo = -1;
		int newIn;
		int newOut;
		//Output redirection
		if(whichDir < 2)
		{
			std::cout << "here";
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
		if(whichDir == 2)
		{
			newIn = dup(0);
			if(newIn == -1)
				perror("DUP IN");
			if(access(command1[newFileplace],F_OK) != -1)
				place = open(command1[newFileplace], O_RDONLY);
			else
				perror("cannot access file");
			
			dupTwo = dup2(place,0);
			
		}
		if(dupTwo == -1)
			perror("DUP2222 ERRORR");
		
		//EXECVP
                getPATH(argv);
		//END EXECVP
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
	int status = 0;
	std::string makeComString = command[0];
/*	if(makeComString.compare("fg") == 0)
	{
		std::cout << "lol";
	}
	else if(makeComString.compare("bg") == 0)
	{
		std::cout << "wtf";
	}*/

	if(makeComString.compare("cd") == 0)
	{
		
		//nothing after cd LOL
		if(command.size() < 2)
		{
			//by itself. we dont have to implement the home cd function. fuck yeah
			std::cout << "DONT HAVE TO IMPLEMENT THIS!!!!!" << std::endl;
			status = -1;
		}
		else
		{
			//make next word a string
			makeComString = command[1];
			if(chdir(command[1]) == -1)
			{
				perror("CD ERROR HAHA");
				status = -1;
			}
			else
				status = 0;
		}
		return status;
	}

	int argc = command.size()+1;				//size of argv
	char **argv = new char*[argc];			//make argv pointer pointer for execvp.
	for(int a = 0;a < argc-1;a++)			//shove everything in command into argv
	{
		argv[a] = command[a];
	}
	argv[argc-1] = '\0';
	//now argv has everything in command	
	int pid = fork();
	child = pid;
	if(pid < 0){
		perror("error with fork(child) ");
		exit(1);
	}
	else if(pid == 0){		//child process running
		//Need to do PATHING. CHANGE EXECVP
		getPATH(argv);
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

int isRedir(std::string checkStr)
{
	if(checkStr.compare(">") == 0)
		return 0;
	else if(checkStr.compare(">>") == 0)
		return 1;
	else if(checkStr.compare("<") == 0)
		return 2;
	else
		return -1;
}

void handler(int i)
{
	//if ^C, current foreground job should interrupt
	if(i == SIGINT)
	{
		std::cout << std::endl;
		if(child > 1)
		{
			if(kill(child,SIGINT) == -1)
				perror("^C error");
		}
		else
			std::cerr << "EXIT to end";
	}
	//^Z, should pause foreground process
	else if(i == SIGTSTP)
	{
		if(kill(child,SIGTSTP))
			//LOL gotta pause the child? WAH DA FA?
			perror("cannot pause your child");
	}		
}

int main()
{
	//^C
	parent = getpid();
	if(SIG_ERR == signal(SIGINT,handler))
		perror("^C error");
	//^Z
	/*if(SIG_ERR == signal(SIGTSTP,handler))
	{
		perror("^Z error");
		exit(1);
	}*/
	
	std::string input;		//string value to get input
	char hostname[100];		//get the hostname from terminal
	char username[100];		//get username from terminal
	gethostname(hostname,100);
	getlogin_r(username,100);

	while(1)			//run until exit(1)
	{
		char currFold[BUFSIZ];
		if(getcwd(currFold,sizeof(currFold)) == NULL)
		{
			perror("ERROR IN CURRENT FOLDER LOL");
			exit(1);
		}
		std::cout << username << "@" << hostname << ":~" << currFold <<"$"; //output beg
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
		int whichDir = -1;
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
				
				else if(convtStr.compare("<") == 0 && chainCom.size() != 0 && firstCommand)
                                {
					bool hasOutput = false;
					std::string convt;
					int whichCon = -1;
					int temp = i;
					char* inputFile = commands[++temp];
					size_t j = i;
					//need to find if there is an output redirection
					while(j < commands.size() && !hasOutput)
					{
						convt = commands[j];
						if(convt.compare(">")==0)
						{
							hasOutput = true;
							whichCon = 0;
						}
						else if(convt.compare(">>")==0)
						{
							hasOutput = true;
							whichCon = 1;
						}
						else
						{
							++j;
							++temp;
						}
					}
	
					if(hasOutput)
					{
						i = temp;
						i++;
						j++;
						char* outputFile = commands[j];
						
						prevCommand = doubleRedirection(chainCom,outputFile,inputFile,whichCon);
					}
					else
					{
						chainCom.push_back(commands[i]);
						i++;
						convtStr = commands.at(i);
						chainCom.push_back(commands[i]);
						prevCommand = singleRedirection(chainCom,2);
						firstCommand = false;
						chainCom.clear();
					}
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
									if(whichDir < 0)
										whichDir = isRedir(convtStr);
                                                                }
                                                                else
                                                                        keepGoing = false;
                                                        }
							if(whichDir > -1)
								prevCommand = singleRedirection(chainCom,whichDir);
							else	
                                                        	prevCommand = syscalls(chainCom);
                                                        keepGoing = true;
							whichDir = -1;
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
										if(whichDir < 0)
											whichDir = isRedir(convtStr);
                                                                	}
                                                                	else
                                                                	        keepGoing = false;
                                                       	 	}	
								if(whichDir > -1)
									prevCommand = singleRedirection(chainCom,whichDir);
								else
									prevCommand = syscalls(chainCom);
								keepGoing = true;
								whichDir = -1;
                                                                chainCom.clear();
                                                        }
							else
                                                        {
                                                                while(check(convtStr) && keepGoing)
                                                                {
                                                                        if((i+1) < commands.size())
                                                                        {
                                                                                i++;
                                                                                convtStr = commands.at(i);
                                                                        }
                                                                        else
                                                                                keepGoing = false;
                                                                }
                                                                prevCommand = false;
                                                        }

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
										if(whichDir < 0)
											whichDir = isRedir(convtStr);
                                                                	}
                                                                	else
                                                                 	       keepGoing = false;
                                                        	}
								for(size_t r = 0;r < chainCom.size();r++)
									std::cout << chainCom[r];
								if(whichDir > -1)
									prevCommand = singleRedirection(chainCom,whichDir);
								else
									prevCommand = syscalls(chainCom);
                                                                keepGoing = true;
								whichDir = -1;
								chainCom.clear();
                                                        }
							else
							{
								while(check(convtStr) && keepGoing)
								{
									if((i+1) < commands.size())
									{
										i++;
										convtStr = commands.at(i);
									}
									else
										keepGoing = false;
								}
								prevCommand = false;
							}
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

					else if(convtStr.compare("<") == 0 && commands.size() != i+1)
                                        {
                                                chainCom.push_back(commands[i]);
                                                i++;
                                                chainCom.push_back(commands[i]);
                                                prevCommand = singleRedirection(chainCom,2);
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
