#pragma once

#include <string>
#include <WinSock2.h>

class Client final
{
public:
	Client(std::string ip, unsigned short port);
	~Client();

public:
	void connectToServer();

private:
	WSADATA _wsaData;
	SOCKET _socket;
	sockaddr_in _serverInfo;

	std::string _serverIp;
	unsigned short _serverPort;

private:
	void winSockInit();
};
