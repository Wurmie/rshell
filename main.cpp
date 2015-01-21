#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>

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
		char *tok = strtok(const_cast<char*>(input.c_str())," &|;"); //will break the words if not &,|,;
		std::vector<char*> commands;				     //vector to store the commands
		while(tok != NULL)					     //loop till end
		{
			commands.push_back(tok);
			tok = strtok(NULL," &|;");
		}		
	}
	return 0;
}
