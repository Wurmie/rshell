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
#include <iomanip>
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
	std::string permission = "          ";
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
	else
		permission[0] = '-';

	if (stuff1 & S_IRUSR) 
		permission[1] = 'r';
	else
		permission[1] = '-';
	if (stuff1 & S_IWUSR) 
		permission[2] = 'w';
	else
                permission[2] = '-';
	if (stuff1 & S_IXUSR) 
		permission[3] = 'x';
	else
                permission[3] = '-';
	if (stuff1 & S_IRGRP) 
		permission[4] = 'r';
	else
                permission[4] = '-';
	if (stuff1 & S_IWGRP) 
		permission[5] = 'w';
	else
                permission[5] = '-';
	if (stuff1 & S_IXGRP) 
		permission[6] = 'x';
	else
                permission[6] = '-';
	if (stuff1 & S_IROTH) 
		permission[7] = 'r';
	else
                permission[7] = '-';
	if (stuff1 & S_IWOTH) 
		permission[8] = 'w';
	else
                permission[8] = '-';
	if (stuff1 & S_IXOTH) 
		permission[9] = 'x';
	else
                permission[9] = '-';
	
	return permission;
	
}
//l = 0, a = 1, R = 2
void outputFormat(std::vector<std::string> fileN,std::vector<bool> Flags,std::string currentPath)
{
	//for(size_t ao = 0; ao < fileN.size();ao++)
	//	std::cout << fileN[ao] << " ";
//	std::cout << std::endl;
	//only need to do -l and -R
	struct stat permissionLines;
	std::string theTime;
	bool Aonly = false;
	std::string newPath;
	size_t TotBlocks = 0;	
	std::string newPath2;

	for(size_t y = 0; y < fileN.size();y++)
	{
		newPath2 = currentPath + "/" + fileN[y];
		if(stat(newPath2.c_str(),&permissionLines) == -1)
		{
			perror("error in blocks");
			exit(1);
		}
		TotBlocks = TotBlocks + permissionLines.st_blocks;
	}
	
	if(Flags[0])
	{
		TotBlocks = TotBlocks / 2;
		std::cout << "total: " << TotBlocks << std::endl;
	}
	if(Flags[2])
	{
		if(fileN.size() == 0)
			fileN.push_back(".");
		
		std::cout << currentPath << ":" << std::endl;
		for(size_t q = 0;q < fileN.size();q++)
			std::cout << fileN[q] << " ";
		std::cout << std::endl << std::endl;
	}
//	if(Flags[2])
//	{
		for(size_t r = 0;r < fileN.size();r++)
		{
			if(!Flags[0] && !Flags[2])
			{
				if(!Aonly)
					Aonly = true;
				std::cout << fileN[r] << " ";
			}	
			else
			{	
				newPath = currentPath + "/" + fileN[r];
		//		std::cout << newPath << std::endl;
				//	newPath = currentPath + "/" + fileN[r];
					//error
				if(stat(newPath.c_str(),&permissionLines) != 0)
				{
					perror("stat error");
					exit(1);
				}
				if(Flags[0])
				{
					//get permission
					std::cout << getPermission(permissionLines)<< std::left << std::setw(2) << " ";
					//# hard links
					std::cout << permissionLines.st_nlink << std::left << std::setw(2) << " ";
					
					struct passwd *user = getpwuid(permissionLines.st_uid);
					if(user == NULL)
					{
						perror("userid invalid");
						exit(1);
					}
					else
						std::cout << user->pw_name << std::left << std::setw(2) << " ";
					struct group *groupid = getgrgid(permissionLines.st_gid);
					if(groupid == NULL)
					{
						perror("group id invalid");
						exit(1);
					}
					else
						std::cout << groupid->gr_name << std::left << std::setw(4) << " ";
					//need error check 
					std::cout << permissionLines.st_size << std::left << std::setw(4) << " ";
					
					theTime = ctime(&permissionLines.st_mtime);
					theTime = theTime.substr(4,theTime.length()-9);
					
					std::cout << theTime << std::left << std::setw(2) << "  ";
					std::cout << fileN[r] << std::left << std::setw(2);
					std::cout << std::endl;
		
				}
			
				if(Flags[2])
				{
					//std::cout << currentPath << ":" << std::endl;
				//	std::cout << fileN[r] << " ";
				//	std::cout << std::endl;
				
					std::vector<std::string> newFile;
					dirent *direntp2;
					//mode_t newStuff = permissionLines.st_mode;
					if(permissionLines.st_mode & S_IFDIR && fileN[r] != "." && fileN[r] != "..")
					{
						DIR *newDir = opendir(newPath.c_str());
						if(newDir == NULL)
							perror("fault opendir");
						//std::cout << fileN[r] << " ";
						while((direntp2 = readdir(newDir)))
						{
							if(direntp2 < 0)
							{
								perror("direntp2 failed");
								exit(1);
							}
							
							char secondDirChar = direntp2->d_name[0];
							//if(newDir != NULL)
							//{
								if(Flags[1])
									newFile.push_back(direntp2->d_name);
								else
								{
									if(secondDirChar != '.')
									{
										newFile.push_back(direntp2->d_name);
									//std::cout << newFile[0];
									}
								}
							//}
						}
						if(closedir(newDir) == -1)
							perror("closedir failed 2");
					//	for(size_t ii = 0; ii < newFile.size();ii++)
					//		std::cout << newFile[ii] << " ";
					//	std::cout << std::endl;
					//	std::sort(newFile.begin(),newFile.end(),howToSort);
						outputFormat(newFile,Flags,newPath);
						newFile.clear();
					}
				//	else
				//	{
				//		closedir(newDir);
				//	}
					//(!Aonly)
					//	Aonly = true;
				}
			}
		}
			if(Aonly)
				std::cout << std::endl;
//	}
//	}
}

bool howToSort(std::string first,std::string second)
{
	if(first[0] == '.')
		first = first.substr(1);
	if(second[0] == '.')
		second = second.substr(1);

	for(size_t m = 0; m < first.size(); m++)
		first[m] = tolower(first[m]);	
	for(size_t n = 0; n < second.size(); n++)
		second[n] = tolower(second[n]);
	return first < second;
	/*
	if(first[0] == '.' && second[0] != '.')	//first has dot, second does not
		return (tolower(first[1]) < tolower(second[0]));
	else if(first[0] == '.' && second[0] == '.') //first has dot and second has dot
		return (tolower(first[1]) < tolower(second[1]));
	else if(first[0] !='.' && second[0] == '.') //first does not and second does
		return (tolower(first[0]) < tolower(second[1]));
	else 						//first does not and second does not
		return (tolower(first[0]) < tolower(second[0]));*/
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
//		std::vector<std::string> multipleF;
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
				if(dirp == NULL)
					perror("file does not exist");
				else
				{
					//check if file exists
					if (theFlags[1])
						filenames.push_back(direntp->d_name);
					else
					{
						if(firstDirChar != '.')
							filenames.push_back(direntp->d_name);
					}
				}
				
			}
//			closedir(dirp);
			if(closedir(dirp) == -1)
			{
				perror("closedir failed");
			}

			std::sort(filenames.begin(),filenames.end(),howToSort);	
//			for(size_t u = 0;u < filenames.size();u++)
//				std::cout << filenames[u] << " ";
			outputFormat(filenames,theFlags,paths[x]);
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
