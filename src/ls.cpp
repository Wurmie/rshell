#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>

//turn flags on
void whichFlags(char* command,std::vector<bool> &flaggs)
{
	//find size of command
	size_t counter = 0;
	for(size_t s = 0;command[s] != '\0';s++)
		counter++;
	
	bool isValid = true;
	size_t k = 0;
	//goes through the command and sees if valid. If not a valid command,nothing gets turned on
	while(isValid && k < counter)
	{
		if(command[k] != 'l' || command[k] != 'R' || command[k] != 'a' || command[k] != '-')
			isValid = false;
	}
	
	//if the command is valid
	if(isValid)
	{
		for(size_t j = 1;j < counter;j++)
		{
			if(command[j] == 'l' && flaggs[0] == false)
				flaggs[0] = true;
			else if(command[j] == 'a' && flaggs[1] == false)
				flaggs[1] = true;
			else if(command[j] == 'R' && flaggs[2] == false)
				flaggs[2] = true;
		}
	}
}

//get an array of char pointers(arrays)
int main(int argc,char** argv)
{
	size_t newArgc = argc;
	std::string comm;
//	for(size_t h = 1;h < newArgc;h++)
//		std::cout << argv[h] << " ";
	if(argc < 1)	//there are no commands
	{
		perror("Error: Something went wrong");
		exit(1);
	}
	else	//something was inputted
	{	
		std::vector<bool>theFlags(3,false);	//initialize flag. 1=-l,2=-a,3=-R
	
		//check for path or flag
		//first word needs to be ls or cannot work
		comm = argv[0];
		if(comm == "ls")
		{
			for(size_t i = 1;i < newArgc;i++)
			{
				comm = argv[i];
				if(comm[i] == '-')	//if find -, then it is a flag
				{
					whichFlags(argv[i],theFlags);
				}
				else if()	//if a path
				{
				}
				else		//if not any
				{
				}
			}
		}
	}	
	/*
	char *dirName = ".";
	DIR *dirp = opendir(dirName);
	dirent *direntp;
	while ((direntp = readdir(dirp)))
		std::cout << direntp->d_name << std::endl; // use stat here to find attributes of file
	closedir(dirp);*/
	return 0;
}
