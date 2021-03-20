#include <iostream>

#include "Client.h"

int _cdecl main(int argc, char** argv)
{
	Client client(argv[1]);
	std::cout << argv[1];
	std::string input;
	while(input!="exit")
	{
		std::getline(std::cin,input);
	}
	return 0;
}