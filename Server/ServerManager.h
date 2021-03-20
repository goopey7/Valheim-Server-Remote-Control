#pragma once
#include <thread>

class ServerManager
{
public:
	void Join();
	void Leave();
	void StartServer();
	void StopServer();
	void ShutdownPC();
	unsigned int GetConnectedClients();
	bool IsShutdownEnabled();
	void SetShutdown(bool shutdown);
	std::thread SpawnServerThread();
private:
	unsigned int connectedClients = 0u;
	bool bShutdown = false;
	std::thread server;
};
