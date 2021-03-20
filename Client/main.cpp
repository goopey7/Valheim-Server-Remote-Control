#include <iostream>

#include "Client.h"

int _cdecl main(int argc, char** argv)
{
	std::cout << "Type 'exit' to leave the server" << std::endl;
	Client client(argv[1]);
	std::string input;
	while(input!="exit")
	{
		std::getline(std::cin,input);
	}
	return 0;
}