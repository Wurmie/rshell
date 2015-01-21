#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>

int main()
{
	bool exit = false;
	std::string input;
	char hostname[128];
	char username[128];

	gethostname(hostname,128);
	getlogin_r(username,128);
	while(!exit)
	{
		std::cout << username << "@" << hostname << "$";
		getline(std::cin,input);
	}
}
