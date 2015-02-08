#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <algorithm>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <cstring>

//turn flags on
bool whichFlags(char* command,std::vector<bool> &flaggs)
{
	//find size of command
	size_t counter = 0;
	for(size_t s = 0;command[s] != '\0';s++)
		counter++;
	bool isValid = true;
	size_t k = 1;
	//goes through the command and sees if valid. If not a valid command,nothing gets turned on
	//first char needs to be -
	if(command[0] != '-')
		isValid = false;
	while(isValid && k < counter)
	{
		if(command[k] != 'l' && command[k] != 'R' && command[k] != 'a')
		{
			isValid = false;
		}
		k++;
	}
	//if the command is valid
	if(isValid)
	{
		//starts from second command. first should be '-'
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
	return isValid;
}

std::string getPermission(struct stat& stuff)
{
	std::string permission = "----------";
	mode_t stuff1 = stuff.st_mode;
	if (S_ISDIR(stuff1))
		 permission[0] = 'd';
	else if (S_ISBLK(stuff1))
		 permission[0] = 'b';
	else if (S_ISCHR(stuff1)) 
		permission[0] = 'c';
	else if (S_ISLNK(stuff1)) 
		permission[0] = 'l';
	else if (S_ISFIFO(stuff1)) 
		permission[0] = 'p';
	else if (S_ISSOCK(stuff1)) 
		permission[0] = 's';

	if (stuff1 & S_IRUSR) 
		permission[1] = 'r';
	if (stuff1 & S_IWUSR) 
		permission[2] = 'w';
	if (stuff1 & S_IXUSR) 
		permission[3] = 'x';
	if (stuff1 & S_IRGRP) 
		permission[4] = 'r';
	if (stuff1 & S_IWGRP) 
		permission[5] = 'w';
	if (stuff1 & S_IXGRP) 
		permission[6] = 'x';
	if (stuff1 & S_IROTH) 
		permission[7] = 'r';
	if (stuff1 & S_IWOTH) 
		permission[8] = 'w';
	if (stuff1 & S_IXOTH) 
		permission[9] = 'x';
	
	return permission;
	
}
void outputFormat(std::vector<std::string> fileN,std::vector<bool> Flags)
{
	//only need to do -l and -R
	struct stat permissionLines;
	std::string theTime;
	bool Aonly = false;
	for(size_t r = 0;r < fileN.size();r++)
	{
		if(!Flags[0] && !Flags[2])
		{
			if(!Aonly)
				Aonly = true;
			std::cout << fileN[r] << " ";
		}
		if(Flags[2])
		{
			while()
			{
				if(Flags[0])
				{	
					//error
					if(stat(fileN[r].c_str(),&permissionLines) != 0)
					{
						perror("stat error");
						exit(1);
					}
					//get permission
					std::cout << getPermission(permissionLines) << " ";
					//# hard links
					std::cout << permissionLines.st_nlink << " ";
					
					struct passwd *user = getpwuid(permissionLines.st_uid);
					if(user == NULL)
					{
						perror("userid invalid");
						exit(1);
					}
					else
						std::cout << user->pw_name << " ";
					struct group *groupid = getgrgid(permissionLines.st_gid);
					if(groupid == NULL)
					{
						perror("group id invalid");
						exit(1);
					}
					else
						std::cout << groupid->gr_name << " ";
					//need error check 
					std::cout << permissionLines.st_size << " ";
					
					theTime = ctime(&permissionLines.st_mtime);
					theTime = theTime.substr(4,theTime.length()-9);
					
					std::cout << theTime << " ";
					std::cout << fileN[r];
					std::cout << std::endl;
				}
			}
		}
	}
	if(Aonly)
		std::cout << std::endl;
}

//get an array of char pointers(arrays)
int main(int argc,char** argv)
{
	bool canFlag;
	size_t newArgc = argc;
	std::string comm;
//	for(size_t h = 1;h < newArgc;h++)
//		std::cout << argv[h] << " ";
	if(argc < 1)	//there are no commands
	{
		perror("Error: Something went wrong");
		exit(1);
	}
	else if(newArgc > 1)	//something was inputted
	{		
		std::vector<bool>theFlags(3,false);	//initialize flag. 1=-l,2=-a,3=-R
		std::vector<std::string> paths;		//these are all the paths
		//check for path or flag
		//first word needs to be ls or cannot work and cannot be just bin/ls
		comm = argv[1];
		if(comm == "ls")
		{
			for(size_t i = 2;i < newArgc;i++)
			{
				comm = argv[i];
				if(comm[0] == '-')	//if find -, then it is a flag
				{
					canFlag = whichFlags(argv[i],theFlags);
					if(!canFlag)
					{
						perror("bad input");
						exit(1);
					}
				}
				else		//it is a path(right or wrong)
				{
					paths.push_back(comm);
				}
			}
		}	
//		for(int z = 0;z < 3;z++)
  //              	std::cout << theFlags[z] << std::endl;
		
		//start to open the paths...somehow
		if(paths.size() == 0) //nothing in there
			paths.push_back(".");
		
//		for(size_t e = 0; e < paths.size();e++)
//			std::cout << paths[e] << " ";
		dirent *direntp;
		std::vector<std::string> filenames;
		std::vector<std::string> multipleF;
		//put -R into a different vector and put that through to the function
		//somehow open each directory
		//go through the paths?
		for(size_t x = 0; x < paths.size();x++)
		{
			if(paths.size() > 1)
				std::cout << paths[x] << ":" << std::endl;
			
			DIR *dirp = opendir(paths[x].c_str());
			if(dirp == NULL)
			{
				perror("opendir failed");
				exit(1);
			}
			
			while((direntp = readdir(dirp)))
			{
				if(direntp <  0)
				{
					perror("direntp failed");
					exit(1);
				}
				//sort out -a now
				char firstDirChar = direntp->d_name[0];
				if(theFlags[1])
					filenames.push_back(direntp->d_name);
				else
				{
					if(firstDirChar != '.')
						filenames.push_back(direntp->d_name);
				}
				
			}
//			closedir(dirp);
			if(closedir(dirp) == -1)
				perror("closedir failed");

			std::sort(filenames.begin(),filenames.end());	
	//		for(size_t u = 0;u < filenames.size();u++)
	//			std::cout << filenames[u] << " ";
			outputFormat(filenames,theFlags);
			filenames.clear();
		}
	}
	else	//nothing input. only bin/ls
		exit(1);
	
	/*
	char *dirName = ".";
	DIR *dirp = opendir(dirName);
	dirent *direntp;
	while ((direntp = readdir(dirp)))
		std::cout << direntp->d_name << std::endl; // use stat here to find attributes of file
	closedir(dirp);*/
	return 0;
}
