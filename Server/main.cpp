#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>


#include "ServerManager.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

WSADATA wsaData;
int iResult;

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;

struct addrinfo* result = NULL;
struct addrinfo hints;

char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;

void OpenSocket(ServerManager &server, std::vector<std::thread> &sockets);

void ProcessMessage(ServerManager &server, std::string &message, std::vector<std::thread> &sockets)
{
	if(message == "JOIN")
	{
		server.Join();
		sockets.emplace_back(OpenSocket, std::ref(server), std::ref(sockets));
	}
	else if(message == "LEAVE")
		server.Leave();
}

void ShellInput(std::string &command, ServerManager &server)
{
	while(true)
	{
		std::getline(std::cin, command);
		if(command == "exit")
			exit(0);
		if(command == "list")
		{
			std::cout << "Clients connected: " << server.GetConnectedClients() << std::endl;
			command = "";
		}
		else if(command == "shutdown")
		{
			if(!server.IsShutdownEnabled())
			{
				server.SetShutdown(true);
				std::cout << "PC will shutdown when everyone has disconnected" << std::endl;
			}
			else
			{
				server.SetShutdown(false);
				std::cout << "PC will NOT shutdown when everyone has disconnected" << std::endl;
			}
		}
	}
}

int __cdecl main(void)
{
	ServerManager server;
	std::string command;
	std::thread input(ShellInput, std::ref(command), std::ref(server));
	std::vector<std::thread> openSockets;
	openSockets.push_back(std::thread(OpenSocket, std::ref(server), std::ref(openSockets)));
	input.join();
	return 0;
}


void OpenSocket(ServerManager &server, std::vector<std::thread> &sockets)
{	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if(iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	
	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if(iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);
	
	iResult = listen(ListenSocket, SOMAXCONN);
	if(iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if(ClientSocket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	// No longer need server socket
	closesocket(ListenSocket);

	// Receive until the peer shuts down the connection
	do
	{
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if(iResult > 0)
		{
			std::string message = std::string(recvbuf, recvbuf + iResult);
			ProcessMessage(server, message, sockets);
		}
		else if(iResult == 0)
			printf("Connection closing...\n");
		else
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}
	}
	while(iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if(iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
}
