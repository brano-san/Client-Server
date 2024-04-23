#include "Server.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <WS2tcpip.h>

Server::Server(std::string ip, unsigned short port)
	: _serverIp(std::move(ip)), _serverPort(port)
{
	ZeroMemory(&(this->_wsaData),      sizeof(WSADATA));
	ZeroMemory(&(this->_serverSocket),       sizeof(SOCKET));
	ZeroMemory(&(this->_serverInfo),   sizeof(sockaddr_in));

	ZeroMemory(&(this->_clientSocket), sizeof(SOCKET));

	this->winSockInit();
}

Server::~Server()
{
	closesocket(this->_clientSocket);
	closesocket(this->_serverSocket);
	WSACleanup();
}

void Server::listenToClient()
{
	if (listen(this->_serverSocket, SOMAXCONN) != 0)
	{
		throw std::runtime_error("Cannot listen to client: " +
			std::to_string(WSAGetLastError()));
	}

	const auto clientInfo = reinterpret_cast<sockaddr*>(&this->_clientInfo);
	int clientSize = sizeof(sockaddr_in);
	this->_clientSocket = accept(this->_serverSocket, clientInfo, &clientSize);
	if (this->_clientSocket == INVALID_SOCKET)
	{
		throw std::runtime_error("Cannot initialize Client Socket: " +
			std::to_string(WSAGetLastError()));
	}


	std::vector<char> message(50);

	recv(this->_clientSocket, message.data(), message.size(), 0);

	for (auto i : message)
		std::cout << i;
	std::cout << '\n';
}

void Server::winSockInit()
{
	if (WSAStartup(MAKEWORD(2, 2), &this->_wsaData) != 0)
	{
		throw std::runtime_error("Cannot initialize WinSock: " +
			std::to_string(WSAGetLastError()));
	}

	this->_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_serverSocket == INVALID_SOCKET)
	{
		throw std::runtime_error("Cannot initialize Server Socket: " +
			std::to_string(WSAGetLastError()));
	}

	in_addr serverIp;
	if (inet_pton(AF_INET, _serverIp.c_str(), &serverIp) <= 0)
	{
		throw std::runtime_error("Cannot cast ip: " +
			std::to_string(WSAGetLastError()));
	}

	this->_serverInfo.sin_family = AF_INET;
	this->_serverInfo.sin_port = htons(this->_serverPort);
	this->_serverInfo.sin_addr = serverIp;

	const auto serverInfo = reinterpret_cast<sockaddr*>(&this->_serverInfo);
	if (bind(this->_serverSocket, serverInfo, sizeof(sockaddr_in)))
	{
		throw std::runtime_error("Cannot bind socket to server: " +
			std::to_string(WSAGetLastError()));
	}
}
