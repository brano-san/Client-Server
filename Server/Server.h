#pragma once

#include <string>
#include <WinSock2.h>

class Server final
{
public:
	Server(std::string ip, unsigned short port);
	~Server();

public:
	void listenToClient();

private:
	WSADATA _wsaData;
	SOCKET _serverSocket;
	sockaddr_in _serverInfo;

	std::string _serverIp;
	unsigned short _serverPort;

	SOCKET _clientSocket;
	sockaddr_in _clientInfo;

private:
	void winSockInit();
};
