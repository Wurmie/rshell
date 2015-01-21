#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <queue>

int main()
{
	bool exit = false;
	std::string input;		//string value to get input
	char hostname[128];		//get the hostname from terminal
	char username[128];		//get username from terminal
	gethostname(hostname,128);
	getlogin_r(username,128);
	while(!exit)			//run until exit is found
	{
		std::cout << username << "@" << hostname << "$"; //output beg
		getline(std::cin,input);			//get input
		std::queue<char> connector;			//queue of connector
		
		for(int i = 0;i < input.size();i++)
		{
			if(input[i].compare(";")== 0) connector.push(';');
		}		

		char *tok = strtok(const_cast<char*>(input.c_str()),";&|"); //will break the words if not a space
		std::queue<char*> commands;				     //queue to store the commands
		while(tok != NULL)					     //loop till end
		{
			commands.push(tok);
			tok = strtok(NULL,";&|");
		}
	/*	while(commands.size() > 0)				    //loop through the words
		{
			char* theWord = commands.front();		    //get the word
			for(int i = 0;i < theWord.size();i++)
			{
				if(
			}
		}*/	
	}
	return 0;
}
