#pragma once

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class Client
{
public:
	Client(const char* ipAddr);
	~Client();
	bool IsConnected();

private:
	bool Send(const char* message);
	bool bConnected = false;

private:

};
