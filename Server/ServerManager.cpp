#include "ServerManager.h"

#include <cstdlib>

void ServerManager::Join()
{
	if(connectedClients == 0)
		server = SpawnServerThread();
	connectedClients++;
}

void ServerManager::Leave()
{
	connectedClients--;
	if(connectedClients == 0)
		StopServer();
}

void ServerManager::StartServer()
{
	system("start.bat");
}

std::thread ServerManager::SpawnServerThread()
{
	return std::thread(&ServerManager::StartServer,this);
}

void ServerManager::StopServer()
{
	system("taskkill /F /T /IM valheim_server.exe");
	server.join();
	if(bShutdown)
		ShutdownPC();
}

void ServerManager::ShutdownPC()
{
	system("C:\\windows\\system32\\shutdown /s");
}

unsigned ServerManager::GetConnectedClients()
{
	return connectedClients;
}

bool ServerManager::IsShutdownEnabled()
{
	return bShutdown;
}

void ServerManager::SetShutdown(bool shutdown)
{
	bShutdown=shutdown;
}
